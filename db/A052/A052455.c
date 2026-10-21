/*
 * A052455  Fixed points for operation of repeatedly replacing a number with
 *          the sum of the fourth power of its digits.
 * https://oeis.org/A052455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, full, nonn, fini
 * Formula:   a(n) = the n‑th fixed point of the operation “replace a number
 *            with the sum of the fourth powers of its decimal digits”. The
 *            fixed points are the (base‑10) Armstrong numbers of order 4:
 *            0,1,1634,8208,9474. The sequence contains these numbers in
 *            ascending order, one per index starting at 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* static storage for the fixed points */
static seq_t fixed_vals[10] = {0};
static idx_t fixed_cnt    = 0;
static bool initialized   = false;

/* compute the fixed points once; max 5 numbers exist for 4th power */
static void compute_fixed(void)
{
    if (initialized) return;

    /* powers of 0..9 to the 4th power */
    const seq_t POW4[10] = {0, 1, 16, 81, 256, 625, 1296, 2401, 4096, 6561};

    /* the largest possible sum for a 5‑digit number  (5·9⁴ = 32805) */
    const idx_t MAX_VAL = 5 * 6561U;
    idx_t count = 0;

    for (idx_t x = 0; x <= MAX_VAL; ++x) {
        seq_t sum = 0;
        idx_t n = x;
        do {
            sum += POW4[n % 10];
            n   /= 10;
        } while (n > 0);

        if (sum == x) {
            if (count < 10) fixed_vals[count] = (seq_t)x;
            ++count;
        }
    }

    fixed_cnt   = count;
    initialized = true;
}

seq_t a(idx_t n)
{
    /* a(1) is the first term, defined for n ≥ 1.  For n < 1 return 0. */
    if (n < 1) return 0;

    compute_fixed();

    if (n <= fixed_cnt) return fixed_vals[n - 1];
    /* no further terms are defined – return 0 as a safe default */
    return 0;
}
