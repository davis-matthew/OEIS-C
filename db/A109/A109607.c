/*
 * A109607  Sum of coprimes of n greater than 1.
 * https://oeis.org/A109607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n*phi(n)/2 - 1 for n>1, where phi(n) is Euler's totient
 *            function. a(0)=0, a(1)=0 as special cases
 * Range:     u8 n <= 28, u16 n <= 366, u32 n <= 7732, u64 n <= 7732,
 *            u128 n <= 7732, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute Euler's totient of n (n>=1) */
static seq_t phi(uint64_t n) {
    seq_t result = (seq_t)n;
    uint64_t i;
    for (i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            /* remove all powers of i */
            while (n % i == 0) n /= i;
            result = result / (seq_t)i * ((seq_t)i - 1);
        }
    }
    if (n > 1) {          /* n is prime now */
        result = result / (seq_t)n * ((seq_t)n - 1);
    }
    return result;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;            /* a(0)=0, a(1)=0 */
    seq_t ph = phi((uint64_t)n);      /* phi(n) fits in seq_t for n up to 10000 */
    seq_t sum = (seq_t)n * ph / 2;    /* sum of coprimes <= n */
    return sum - 1;                   /* exclude the coprime '1' */
}
