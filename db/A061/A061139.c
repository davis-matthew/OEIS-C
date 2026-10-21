/*
 * A061139  Number of degree-n odd permutations of order exactly 6.
 * https://oeis.org/A061139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A061139 – Number of degree‑n odd permutations of order exactly 6.
   The algorithm uses a recurrence that counts permutations whose
   cycle lengths belong to each divisor of 6 (1, 2, 3, 6).  For every
   such class we keep both the even‑parity and odd‑parity counts.
   A permutation obtained by adding a cycle of length i to a
   permutation of size n-i changes its parity exactly when i is even
   (because a cycle of even length contributes one more transposition).
   After the tables for the four divisor classes are built, Möbius
   inversion is applied:

        odd_6(n) = odd_1(n) – odd_2(n) – odd_3(n) + odd_6(n)

   which gives the desired number of odd permutations of order exactly 6. */

#include <stddef.h>   /* size_t */

seq_t a(idx_t n) {
    /* divisor list and allowed cycle lengths for each divisor of 6 */
    static const int divs[4]       = {1, 2, 3, 6};
    static const int cycles[4][4]  =
        { {1, 0, 0, 0},
          {1, 2, 0, 0},
          {1, 3, 0, 0},
          {1, 2, 3, 6} };
    static const int ccount[4]     = {1, 2, 2, 4};

    /* dynamic storage for even/odd counts: ev[d][k], od[d][k] */
    static seq_t *ev[4] = {0, 0, 0, 0};
    static seq_t *od[4] = {0, 0, 0, 0};
    static size_t cap   = 0;          /* current size of tables (entries 0..cap-1) */

    /* ensure tables are large enough for index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = ((size_t)n) + 1;
        /* grow each table to the new capacity */
        for (int d = 0; d < 4; ++d) {
            ev[d] = (seq_t *)realloc(ev[d], new_cap * sizeof(seq_t));
            od[d] = (seq_t *)realloc(od[d], new_cap * sizeof(seq_t));
        }
        /* compute missing rows, from old cap up to new_cap-1 */
        for (size_t k = cap; k < new_cap; ++k) {
            if (k == 0) {            /* base case: identity permutation */
                for (int d = 0; d < 4; ++d) {
                    ev[d][0] = (seq_t)1;
                    od[d][0] = (seq_t)0;
                }
                continue;
            }
            for (int d = 0; d < 4; ++d) {
                seq_t even_sum = (seq_t)0;
                seq_t odd_sum  = (seq_t)0;
                /* iterate over allowed cycle lengths for this divisor */
                for (int c = 0; c < ccount[d]; ++c) {
                    int i = cycles[d][c];
                    if ((size_t)i > k) break;          /* cycle too long */
                    /* coefficient = (k-1)! / (k-i)! =
                       product of integers from (k-i+1) to (k-1) */
                    seq_t coeff = (seq_t)1;
                    if (i > 1) {
                        for (size_t t = k - i + 1; t <= k - 1; ++t)
                            coeff *= (seq_t)t;
                    }
                    /* terms from the remaining n-i elements */
                    seq_t term_even = ev[d][k - i];
                    seq_t term_odd  = od[d][k - i];
                    seq_t prod_even = term_even * coeff;
                    seq_t prod_odd  = term_odd  * coeff;
                    /* flip parity when cycle length is even */
                    if (i % 2 == 0) {   /* even length → sign -1 */
                        even_sum += prod_odd;
                        odd_sum  += prod_even;
                    } else {            /* odd length → sign +1 */
                        even_sum += prod_even;
                        odd_sum  += prod_odd;
                    }
                }
                ev[d][k] = even_sum;
                od[d][k] = odd_sum;
            }
        }
        cap = new_cap;
    }

    /* Möbius inversion: odd permutations of exact order 6 */
    seq_t result = od[3][n];          /* od for divisor 6  */
    result += od[0][n];              /* + od for divisor 1  */
    result -= od[1][n];              /* - od for divisor 2  */
    result -= od[2][n];              /* - od for divisor 3  */
    return result;
}
