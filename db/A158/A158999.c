/*
 * A158999  Palprimes of the form m*reversal(m)+1, sorted on m.
 * https://oeis.org/A158999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_palindrome_base, is_prime_u64, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158999: palprimes of the form m*rev(m)+1,
   sorted on increasing m (duplicates preserved). */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/reverse_digits_idx.h"
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
    if (n == 0)
        return (seq_t)0;                       /* undefined for offset < 1 */
    static idx_t next_m = 1;                  /* next m to test */
    static seq_t *terms = NULL;              /* stored palprime values */
    static size_t term_len = 0, term_cap = 0;

    size_t need = (size_t)n;                  /* target count */

    while (term_len < need) {
        idx_t rev_m = reverse_digits_idx(next_m);
        seq_t m   = (seq_t)next_m;
        seq_t reverse_digits_idx = (seq_t)rev_m;
        seq_t cand = m * reverse_digits_idx + 1;

        if (is_palindrome_base(cand, 10) && is_prime_big(cand)) {
            if (term_len == term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 4;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) {
                    mem_free(terms);
                    exit(1);
                }
                terms = tmp;
                term_cap = new_cap;
            }
            terms[term_len++] = cand;
        }
        ++next_m;
    }
    return terms[need - 1];
}
