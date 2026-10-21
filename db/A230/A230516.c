/*
 * A230516  Number of ways to write n = a + b + c with 0 < a <= b <= c such
 *          that {a^2+a-1, a^2+a+1}, {b^2+b-1, b^2+b+1}, {c^2+c-1, c^2+c+1}
 *          are twin prime pairs.
 * https://oeis.org/A230516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 497, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A230516
 *  Count the number of triples (a,b,c) with 0 < a ≤ b ≤ c and
 *  a + b + c = n such that each of a, b and c yields a twin prime
 *  pair {x²+x−1, x²+x+1}.
 *  The routine pre‑computes a flag array of twin‑prime-producing
 *  numbers up to n and then counts admissible triples in O(n²) time.
 *  It is valid for all n ≥ 1.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static unsigned char *twin_flag = NULL;   /* 0 = not twin‑prime, 1 = yes */
static idx_t twin_flag_max = 0;

/* Ensure twin_flag is allocated and filled up to at least n. */
static void ensure_twin_flag(idx_t n)
{
    if (twin_flag_max >= n) return;

    idx_t new_size = n + 1;                    /* indices 0 .. n */
    unsigned char *tmp = (unsigned char *)realloc(twin_flag, new_size);
    if (!tmp) exit(1);                        /* out of memory */

    for (idx_t x = twin_flag_max + 1; x <= n; ++x) {
        uint64_t value = (uint64_t)x * x + x - 1ULL;  /* x² + x – 1 */
        if (isPrimeGeneric((seq_t)value) &&
            isPrimeGeneric((seq_t)(value + 2ULL)))
            tmp[x] = 1;
        else
            tmp[x] = 0;
    }

    twin_flag = tmp;
    twin_flag_max = n;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    ensure_twin_flag(n);

    seq_t count = 0;

    for (idx_t a_val = 1; a_val <= n; ++a_val) {
        if (!twin_flag[a_val]) continue;

        idx_t rem = n - a_val;
        if (rem < 2) continue;          /* need at least b + c = 2 */

        idx_t max_b = rem / 2;          /* guarantees b ≤ c */

        for (idx_t b_val = a_val; b_val <= max_b; ++b_val) {
            if (!twin_flag[b_val]) continue;

            idx_t c_val = rem - b_val;  /* n − a − b */

            if (c_val < b_val) continue; /* safety, normally impossible */

            if (!twin_flag[c_val]) continue;
            ++count;
        }
    }

    return count;
}
