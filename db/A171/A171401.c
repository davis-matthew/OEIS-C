/*
 * A171401  Numbers m such that exactly one editing step (insert or
 *          substitute) is necessary to transform the binary representation of
 *          m into the least prime not less than m.
 * https://oeis.org/A171401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 700, u32 n <= 700, u64 n <= 700,
 *            u128 n <= 700, big any n
 * Validated: standard, pass, 78 terms through n = 635 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A171401 by brute‑force search.
   a(n) is the n‑th integer m such that the binary Levenshtein
   distance between m and the smallest prime >= m equals 1
   (exactly one insert or substitute).  The algorithm
   keeps a cache of all found values; it grows the cache
   on demand. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/bit_length.h"
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

/* --------------------------------------------------------------------- */
static bool is_substitution_ok(seq_t m, seq_t p)
{
    seq_t diff = m ^ p;
    return diff != (seq_t)0 && (diff & (diff - (seq_t)1)) == (seq_t)0;
}

/* --------------------------------------------------------------------- */
/* helper: a single substitution changes one bit of m into p           */

/* --------------------------------------------------------------------- */
/* helper: p is obtained by inserting one bit into the binary of m      */
/* --------------------------------------------------------------------- */
static bool is_insertion_ok(seq_t m, seq_t p)
{
    idx_t len_m = (m == (seq_t)0) ? (idx_t)1 : bit_length(m);
    idx_t len_p = bit_length(p);
    if (len_p != len_m + 1) return false;

    for (idx_t pos = 0; pos < len_p; ++pos) {
        seq_t low_mask = (pos == 0) ? (seq_t)0
                                    : (((seq_t)1 << pos) - (seq_t)1);
        seq_t high = p >> (pos + 1);
        seq_t low  = p & low_mask;
        seq_t cand = (high << pos) | low;
        if (cand == m) return true;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/* helper: smallest prime >= m                                         */
static seq_t next_prime(seq_t m)
{
    if (m <= (seq_t)2) return (seq_t)2;
    seq_t cand = m;
    if ((cand & (seq_t)1) == (seq_t)0) cand += (seq_t)1;  /* make odd */
    for (;;) {
        if (is_prime_big(cand)) return cand;
        cand += (seq_t)2;
    }
}

/* --------------------------------------------------------------------- */
/* helper: does m satisfy the defining condition?                       */
static bool m_is_valid(seq_t m)
{
    seq_t p = next_prime(m);
    idx_t len_m = (m == (seq_t)0) ? (idx_t)1 : bit_length(m);
    idx_t len_p = bit_length(p);
    return (len_p == len_m) ? is_substitution_ok(m, p)
                            : is_insertion_ok(m, p);
}

/* --------------------------------------------------------------------- */
/* main sequence function                                                */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* out of range, defined to 0 */

    static seq_t *seq = NULL;   /* cache of found values           */
    static size_t  cap  = 0;    /* allocated capacity              */
    static size_t  cnt  = 0;    /* number of elements stored       */
    static idx_t   next = 0;    /* next integer to test            */

    while (cnt < n) {
        for (;;) {
            seq_t m = (seq_t)next;
            ++next;
            if (m_is_valid(m)) {
                if (cnt == cap) {                     /* grow cache    */
                    size_t newcap = (cap) ? cap * 2 : 1;
                    while (newcap <= cnt) newcap <<= 1;
                    seq_t *tmp = mem_new(seq_t, newcap);
                    if (!tmp) abort();                /* allocation fail */
                    if (seq) {
                        mem_copy(tmp, seq, cnt * sizeof(seq_t));
                        mem_free(seq);
                    }
                    seq = tmp;
                    cap = newcap;
                }
                seq[cnt++] = m;
                break;
            }
        }
    }
    return seq[n - 1];
}
