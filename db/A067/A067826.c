/*
 * A067826  Primes p such that sigma(2*p+1) > 3*(p+1).
 * https://oeis.org/A067826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p with sigma(2 p + 1) > 3 · (p + 1); the n‑th such prime is returned. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;      /* number of terms already stored */
    static size_t seq_cap = 0;      /* current capacity of seq array */
    static idx_t last_checked = 1; /* last integer examined */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        idx_t cand = last_checked + 1;

        /* Scan forward until the next qualifying prime is found */
        while (1) {
            last_checked = cand;

            if (is_prime(cand)) {
                idx_t m = 2 * cand + 1;          /* 2 p + 1 */
                seq_t sum = sigma_of(m);         /* σ(m) */
                seq_t thr = (seq_t)3 * (seq_t)(cand + 1); /* 3·(p + 1) */

                if (sum > thr) {                 /* condition satisfied */
                    /* Grow the cache if necessary */
                    if (seq_len == seq_cap) {
                        seq_cap = seq_cap ? seq_cap * 2 : 64;
                        seq = mem_resize(seq_t, seq, seq_cap);
                    }
                    seq[seq_len++] = (seq_t)cand;
                    break;                      /* next term found */
                }
            }
            cand++;  /* try the next candidate */
        }
    }

    return seq[n - 1];   /* 1‑based index */
}
