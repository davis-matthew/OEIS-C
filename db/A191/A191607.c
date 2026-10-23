/*
 * A191607  Table of the difference of numerator and denominator of Farey
 *          sequences, read by rows.
 * https://oeis.org/A191607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, tabf, nonn
 * Formula:   a(n) is the flattened list of differences (denominator –
 *            numerator) of all reduced fractions in the Farey sequence of
 *            each order r, read by rows. The first row corresponds to r = 1,
 *            the second to r = 2, etc. This function computes the n-th term
 *            of that list
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the Euler totient φ(n) for small n (n <= a few thousand). */
static inline idx_t totient(idx_t n)
{
    idx_t result = n;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) {
                tmp /= p;
            }
            result -= result / p;
        }
    }
    if (tmp > 1) {
        result -= result / tmp;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r that contains the n-th term. */
    idx_t r = 1;
    idx_t cumulative = 0;      /* number of terms in all previous rows */
    idx_t sum_phi = 0;         /* cumulative φ(1)+...+φ(r)     */
    while (1) {
        idx_t phi_r = totient(r);
        sum_phi += phi_r;
        idx_t row_len = 1 + sum_phi;           /* length of row r */
        if (cumulative + row_len >= n) {
            idx_t pos_in_row = n - cumulative;  /* 1‑based index within the row */

            /* Generate the Farey sequence of order r and locate the
               pos_in_row‑th term.  b – a is always non‑negative. */
            if (pos_in_row == 1) {           /* the first term 0/1 */
                return (seq_t)1;              /* 1 – 0 */
            }

            /* Standard Farey sequence generation.  a/b is the previous term,
               c/d is the current term (the one we will output). */
            idx_t a_num = 0, b_den = 1;
            idx_t c_num = 1, d_den = r;
            idx_t count = 1;                 /* we have already output 0/1 */

            while (c_num <= r) {
                /* Compute the next term in the sequence. */
                idx_t k = (r + b_den) / d_den;
                idx_t e_num = k * c_num - a_num;
                idx_t f_den = k * d_den - b_den;

                a_num = c_num;
                b_den = d_den;
                c_num = e_num;
                d_den = f_den;

                ++count;
                if (count == pos_in_row) {
                    return (seq_t)b_den - (seq_t)a_num;
                }
            }
            /* The loop always returns before it ends because
               pos_in_row is guaranteed to be <= row_len. */
        }
        /* Move to the next row. */
        cumulative += 1 + sum_phi;   /* add the length of the current row */
        ++r;
    }
}
