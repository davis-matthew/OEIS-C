/*
 * A084688  Nonnegative integers n such that 2^n uses only distinct decimal
 *          digits.
 * https://oeis.org/A084688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full, base
 * Formula:   a(n) = n such that 2^n uses only distinct decimal digits
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * The sequence A084688 has exactly 18 terms.  The function below
 * generates them lazily on the first call and then returns the
 * n-th term (1‑based index).  For indices beyond the length of the
 * sequence it returns 0.
 *
 * The search is trivial because the largest value of n that satisfies
 * the property is 29; 2^30 already has repeated digits.  The function
 * uses only 64‑bit operations on the small values involved.
 */
#include <stdbool.h>

static bool init_done = false;
static seq_t terms[18];

/* Return true if x has all distinct decimal digits. */
static bool distinct_digits(seq_t x) {
    unsigned mask = 0;
    unsigned long long v = (unsigned long long)x; /* safe: x < 2^32 here */
    if (v == 0) {
        return true;      /* 2^n for n=0 yields 1, not 0 */
    }
    while (v) {
        int d = v % 10;
        if (mask & (1u << d)) {
            return false; /* duplicate digit */
        }
        mask |= 1u << d;
        v /= 10;
    }
    return true;
}

seq_t a(idx_t n) {
    if (!init_done) {
        seq_t pow = 1;      /* 2^0 */
        idx_t count = 0;
        idx_t k = 0;
        while (count < 18) {
            if (distinct_digits(pow)) {
                terms[count++] = k;
            }
            /* Prepare next power: 2^(k+1) */
            pow = pow * 2;
            ++k;
        }
        init_done = true;
    }

    if (n == 0) return 0; /* undefined for n<1, guard */
    if (n <= 18) {
        return terms[n - 1];
    }
    /* n is beyond the known terms; the sequence has no further values. */
    return 0;
}
