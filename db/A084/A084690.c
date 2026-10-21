/*
 * A084690  4^n uses only distinct decimal digits.
 * https://oeis.org/A084690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A084690: n such that 4^n uses only distinct decimal digits.
 * The function enumerates powers of 4 until the nine known terms
 * (0,1,2,3,4,5,6,7,10) are found.  Subsequent calls simply
 * look up the cached values.
 */

#include <stdbool.h>
#include <stddef.h>

/* Return true if all decimal digits of v are distinct. */
static bool distinct_digits(seq_t v) {
    unsigned mask = 0;
    while (v > (seq_t)0) {
        seq_t d = v % (seq_t)10;          /* current decimal digit */
        unsigned di = (unsigned)d;
        if (mask & (1u << di))
            return false;                /* duplicate digit found */
        mask |= (1u << di);
        v /= (seq_t)10;                  /* drop last digit */
    }
    /* v == 0 occurs only when the number is 0; a single digit 0 is distinct. */
    return true;
}

seq_t a(idx_t n) {
    static idx_t results[10];
    static size_t count = 0;
    static bool initialized = false;

    if (!initialized) {
        seq_t power = (seq_t)1;          /* 4^0 */
        for (idx_t exp = 0; ; ++exp) {
            if (distinct_digits(power)) {
                if (count < 10)
                    results[count++] = exp;
            }
            if (count >= 9)              /* all terms found */
                break;
            power *= (seq_t)4;           /* next power of 4 */
        }
        initialized = true;
    }

    if (n == 0 || n > count)
        return (seq_t)0;                 /* undefined / out of range */

    return (seq_t)results[n - 1];
}
