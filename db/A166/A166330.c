/*
 * A166330  a(n) = (prime(n+1)+1)^(prime(n+1)-1) - (prime(n)-1)^(prime(n)+1).
 * https://oeis.org/A166330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+1)+1)^(prime(n+1)-1) - (prime(n)-1)^(prime(n)+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* We assume n >= 1.  For n < 1 the behavior is unspecified. */
    uint32_t p_n  = get_prime(n);      /* prime(n)    */
    uint32_t p_np1 = get_prime(n + 1); /* prime(n+1)  */

    seq_t term1 = big_pow((seq_t)(p_np1 + 1), (seq_t)(p_np1 - 1));
    seq_t term2 = big_pow((seq_t)(p_n - 1), (seq_t)(p_n + 1));

    return term1 - term2;
}
