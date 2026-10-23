/*
 * A130680  Numbers n such that n = (a_1 + a_2 + ... + a_p)*(a_1^3 + a_2^3 +
 *          ... + a_p^3), where n has the decimal expansion a_1a_2...a_p.
 * https://oeis.org/A130680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   Numbers n such that n = (sum of decimal digits of n) * (sum of
 *            cubes of decimal digits of n); This function returns the nth
 *            such number (1‑based)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* forward declaration of the function required by the test harness */
seq_t a(idx_t n);

/* internal storage of the sequence terms */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* initialise the sequence list once */
static void init_sequence(void)
{
    if (seq != NULL)    /* already initialised */
        return;

    /* start with a small capacity, grow as needed */
    seq_cap = 256;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (seq == NULL)
        return;        /* allocation failed, give up */

    /* brute‑force all 6‑digit or fewer numbers; 10^6 is trivial */
    for (idx_t num = 1; num <= 999999; ++num) {
        unsigned int sum = 0, sum_cubes = 0;
        idx_t tmp = num;
        while (tmp > 0) {
            unsigned int d = tmp % 10;
            sum += d;
            sum_cubes += d * d * d;   /* d ≤ 9, so fits in 32‑bit */
            tmp /= 10;
        }
        idx_t prod = (idx_t)sum * (idx_t)sum_cubes;
        if (prod == num) {
            /* store the number */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_t *newp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (newp == NULL)
                    return;            /* allocation failed, give up */
                seq = newp;
            }
            seq[seq_len++] = (seq_t)num;
        }
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for 0, return 0 as sentinel */

    /* ensure the sequence is built */
    if (seq == NULL)
        init_sequence();

    if (n - 1 < seq_len)
        return seq[n - 1];

    return 0;   /* n is beyond the known terms */
}
