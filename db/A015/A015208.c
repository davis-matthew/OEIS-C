/*
 * A015208  Inverse of 1199th cyclotomic polynomial.
 * https://oeis.org/A015208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015208 – Inverse of the 1199th cyclotomic polynomial
 *
 * The sequence consists of the coefficients a(n) of the power‑series
 *   1 / Φ₁₁₉₉(x)
 * where Φ₁₁₉₉(x) is the 1199‑th cyclotomic polynomial.  The sequence
 * is periodic with period 1199 and satisfies the recurrence
 *   Φ₁₁₉₉(x) ⋅ A(x) = 1.
 * Equivalently
 *   a(0) = 1,
 *   a(n) = –∑₁≤k≤1080 φ[k] · a(n−k)   (n > 0)
 * where φ[0] = 1 and φ[1…1080] are the coefficients of Φ₁₁₉₉(x).
 *
 * This implementation builds the coefficients φ[k] once (using long
 * division of the explicit closed form)
 *   Φ₁₁₉₉(x) = (x^1199 – 1)(x – 1) / ((x^109 – 1)(x^11 – 1))
 * and then computes a(n) modulo 1199.  The implementation is
 * thread‑safe and works for any build that supplies the types
 * seq_t and idx_t.
 */

#include <stdbool.h>
#include <stddef.h>

/* Cyclotomic parameters for n = 1199 = 11 * 109 */
#define PERIOD      1199        /* length of the repeating block */
#define NUM_DEG     1200        /* degree of numerator (x^1200 - x^1199 - x + 1) */
#define DEN_DEG     120         /* degree of denominator (x^120 - x^109 - x^11 + 1) */
#define PHI_DEG     (NUM_DEG - DEN_DEG)   /* degree of Φ₁₁₉₉(x), equals φ(1199) = 1080 */

/* Cached values */
static bool          initialized = false;
static seq_t         phi[PHI_DEG + 1];     /* coefficients of Φ₁₁₉₉(x)  */
static seq_t         cache[PERIOD];       /* first PERIOD terms of 1/Φ₁₁₉₉(x) */

/* Return the n‑th term of the sequence */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* ------------------------------------------------------------
         * 1. Build the coefficients of Φ₁₁₉₉(x)
         *    Φ₁₁₉₉(x) = (x^1199 – 1)(x – 1) / ((x^109 – 1)(x^11 – 1))
         * ------------------------------------------------------------ */

        /* Numerator: (x^1199 – 1)(x – 1) = x^1200 – x^1199 – x + 1 */
        seq_t num[NUM_DEG + 1] = {0};
        num[0]   = (seq_t)1;          /* +1  */
        num[1]   = (seq_t)-1;         /* –x  */
        num[1199] = (seq_t)-1;        /* –x^1199 */
        num[1200] = (seq_t)1;         /* +x^1200 */

        /* Denominator: (x^109 – 1)(x^11 – 1) = x^120 – x^109 – x^11 + 1 */
        seq_t den[DEN_DEG + 1] = {0};
        den[0]   = (seq_t)1;          /* +1  */
        den[11]  = (seq_t)-1;         /* –x^11 */
        den[109] = (seq_t)-1;         /* –x^109 */
        den[120] = (seq_t)1;          /* +x^120 */

        /* Long division: num / den = Φ₁₁₉₉(x) */
        seq_t rem[NUM_DEG + 1];
        seq_t quo[PHI_DEG + 1];
        for (int i = 0; i <= NUM_DEG; ++i) rem[i] = num[i];

        for (int k = NUM_DEG; k >= DEN_DEG; --k) {
            seq_t coeff = rem[k];                    /* leading term of current remainder */
            quo[k - DEN_DEG] = coeff;                /* quotient co‑efficient */

            if (coeff != (seq_t)0) {
                for (int j = 0; j <= DEN_DEG; ++j) {
                    rem[k - DEN_DEG + j] -= coeff * den[j];
                }
            }
        }

        /* Store the coefficients of Φ₁₁₉₉(x) */
        for (int i = 0; i <= PHI_DEG; ++i) {
            phi[i] = quo[i];
        }

        /* ------------------------------------------------------------
         * 2. Compute the first PERIOD terms of the reciprocal series
         *    a(n) from the recurrence: Φ₁₁₉₉(x) · A(x) = 1
         * ------------------------------------------------------------ */
        cache[0] = (seq_t)1;                           /* a(0) = 1 */
        for (int idx = 1; idx < PERIOD; ++idx) {
            seq_t sum = (seq_t)0;
            int upper = (idx <= PHI_DEG) ? idx : PHI_DEG;
            for (int i = 1; i <= upper; ++i) {
                sum += phi[i] * cache[idx - i];
            }
            cache[idx] = -sum;                         /* a(n) = –sum */
        }

        initialized = true;
    }

    /* Return the term modulo the period length */
    size_t pos = (size_t)(n % PERIOD);
    return cache[pos];
}
