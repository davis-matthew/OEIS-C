/*
 * A190298  Numbers less than or equal to sum of the triangular number of each
 *          of their decimal digits.
 * https://oeis.org/A190298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A190298: a(n) = the n‑th integer m such that
   m <= Σ_{d∈digits(m)} T(d), with T(d)=d(d+1)/2.
   The sequence is finite (20 terms). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t terms[20];
    static size_t term_count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* 3 decimal digits ⇒ max ΣT(d) = 45*3 = 135 */
        for (idx_t x = 1; x <= 135 && term_count < 20; ++x) {
            seq_t sum = 0;
            idx_t temp = x;
            while (temp != 0) {
                uint8_t d = (uint8_t)(temp % 10);
                sum += ((seq_t)d * (d + 1)) / 2;
                temp /= 10;
            }
            if ((seq_t)x <= sum) {
                terms[term_count++] = (seq_t)x;
            }
        }
        initialized = true;
    }

    if (n < 1 || n > (idx_t)term_count) {
        return 0; /* undefined for indices beyond the sequence length */
    }

    return terms[n - 1];
}
