/*
 * A207338  Triangle read by rows in which row n lists the prime factors of n
 *          with repetition, with a(1) = 1, but with the primes of the form 4k
 *          + 1 multiplied by -1.
 * https://oeis.org/A207338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = the array element obtained by printing the prime
 *            factorization (with repetitions) of each integer starting at 1,
 *            modifying each prime factor congruent to 1 mod 4 to its
 *            negation, and then reading all elements row‑by‑row. For example
 *            a(6) = -5 because the row for n = 5 is [‑5] and it occupi...
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;      /* a(1) = 1 */

    idx_t target = n;
    idx_t cum = 1;           /* terms from row 1 */
    idx_t r = 2;             /* current integer whose row we are processing */

    /* Find the row r that contains the target index */
    while (cum < target) {
        idx_t cnt = 0;       /* number of prime factors of r (with multiplicity) */
        idx_t m = r;
        /* trial division to count prime factors */
        idx_t p = 2;
        while (p * p <= m) {
            while (m % p == 0) {
                ++cnt;
                m /= p;
            }
            ++p;
        }
        if (m > 1) ++cnt;   /* remaining prime */

        cum += cnt;
        ++r;                /* advance to next integer */
    }
    --r;                    /* r is now the row number containing the target */

    /* Count prime factors again for this row (to compute offset) */
    idx_t cnt_row = 0;
    idx_t m = r;
    idx_t p = 2;
    while (p * p <= m) {
        while (m % p == 0) {
            ++cnt_row;
            m /= p;
        }
        ++p;
    }
    if (m > 1) ++cnt_row;

    /* Compute 0‑based index of the factor within the row */
    idx_t offset = target - (cum - cnt_row) - 1;

    /* Retrieve the factor at that offset */
    idx_t t = r;
    idx_t idx_factor = 0;
    idx_t factor = 0;
    p = 2;
    while (p * p <= t) {
        while (t % p == 0) {
            if (idx_factor == offset) {
                factor = p;
                break;
            }
            ++idx_factor;
            t /= p;
        }
        if (factor != 0)
            break;
        ++p;
    }
    if (factor == 0 && t > 1) {          /* last remaining prime factor */
        if (idx_factor == offset)
            factor = t;
    }

    /* Apply sign rule: primes ≡ 1 (mod 4) become negative */
    if (factor % 4 == 1)
        return (seq_t)(-(int64_t)factor);
    else
        return (seq_t)factor;
}
