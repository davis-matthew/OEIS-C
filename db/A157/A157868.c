/*
 * A157868  Palindromic primes with prime digital roots.
 * https://oeis.org/A157868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth palindromic prime with prime digital root
 * Range:     u8 n <= 9, u16 n <= 38, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   digits, has_prime_droot, isPrimeGeneric, pow10, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A157868 */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/reverse_decimal.h"
#include "../utils/pow10.h"
#include "../utils/digits.h"
#include "../utils/has_prime_droot.h"

static inline seq_t make_pal(seq_t half, bool odd) {
    /* number of decimal digits of half */
    idx_t len = digits((idx_t)half);
    seq_t rev, pow;
    if (odd) {
        seq_t half_div10 = half / 10u;
        rev = reverse_decimal(half_div10);
        pow = pow10(len - 1);
    } else {
        rev = reverse_decimal(half);
        pow = pow10(len);
    }
    return half * pow + rev;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1    */
    idx_t count = 0;

    for (idx_t len = 1; ; ++len) {
        bool odd = (len & 1);
        idx_t half_len = (len + 1) / 2;

        /* smallest half: 10^(half_len-1), largest: 10^half_len - 1 */
        idx_t start = 1;
        for (idx_t i = 1; i < half_len; ++i) start *= 10;
        idx_t end   = start * 10 - 1;

        for (idx_t half = start; half <= end; ++half) {
            seq_t pal = make_pal((seq_t)half, odd);
            if (isPrimeGeneric(pal) && has_prime_droot(pal)) {
                ++count;
                if (count == n) return pal;
            }
        }
    }
}
