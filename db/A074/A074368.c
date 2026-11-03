/*
 * A074368  Rearrangement of odd numbers such that a(k) + a(k+1) + 1 is a
 *          prime for all k.
 * https://oeis.org/A074368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for k≥1, a(k+1) is the smallest unused odd integer *
 *            such that a(k)+a(k+1)+1 is prime. This greedy construction
 *            yields * OEIS A074368. * Includes memoization so that calls may
 *            arrive in any order
 * Range:     u8 n <= 63, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *seq      = (seq_t*)NULL;  /* generated terms */
    static size_t seq_len  = 0;             /* number of terms stored */
    static size_t seq_cap  = 0;             /* capacity of seq array */

    static bool *used_odds = (bool*)NULL;   /* marks used odd numbers */
    static size_t used_cap  = 0;             /* capacity of used_odds */

    static seq_t last_used = 0;              /* most recently added term */
    static bool initialized = false;

    /* first call: initialize */
    if (!initialized) {
        /* initial capacity (will grow as needed) */
        seq_cap   = 128;
        seq       = mem_new(seq_t, seq_cap);
        if (!seq) return (seq_t)0;            /* allocation failure */

        seq[0] = (seq_t)1;
        seq_len = 1;
        last_used = (seq_t)1;

        used_cap  = 1;                       /* odd 1 corresponds to index 0 */
        used_odds = (bool*)malloc(used_cap * sizeof(bool));
        if (!used_odds) { mem_free(seq); return (seq_t)0; }
        memset(used_odds, 0, used_cap * sizeof(bool));
        used_odds[0] = true;                 /* mark 1 as used */

        initialized = true;
    }

    /* extend sequence until we have at least n terms */
    while (seq_len < (size_t)n) {
        seq_t last = last_used;
        seq_t candidate = 3;                 /* start from the next odd */

        for (;;) {
            /* index of candidate in used_odds array */
            size_t idx = (size_t)(candidate - (seq_t)1) / 2;

            /* resize used_odds if necessary */
            if (idx >= used_cap) {
                size_t old_cap = used_cap;
                used_cap = (idx + 1) * 2;
                bool *tmp = (bool*)realloc(used_odds, used_cap * sizeof(bool));
                if (!tmp) { mem_free(seq); free(used_odds); return (seq_t)0; }
                /* zero new portion */
                memset(tmp + old_cap, 0, (used_cap - old_cap) * sizeof(bool));
                used_odds = tmp;
            }

            /* check unused and primality condition */
            if (!used_odds[idx] &&
                is_prime_big(last + candidate + (seq_t)1)) {

                /* ensure seq array has space */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, seq, new_cap);
                    if (!tmp) { mem_free(seq); free(used_odds); return (seq_t)0; }
                    seq = tmp;
                    seq_cap = new_cap;
                }

                seq[seq_len++] = candidate;
                last_used = candidate;
                used_odds[idx] = true;
                break;
            }

            candidate += 2;   /* next odd number */
        }
    }

    return seq[n - 1]; /* sequence is 0‑indexed internally */
}
