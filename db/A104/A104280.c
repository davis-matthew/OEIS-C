/*
 * A104280  Numbers n such that 2n+1 is not prime and 2n-1 is prime.
 * https://oeis.org/A104280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of A104280:
   numbers n such that 2n+1 is not prime and 2n-1 is prime.
   This implementation searches integers sequentially, testing the
   primality conditions with is_prime from utils/is_prime.h. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

#if defined(__cplusplus)
#   define SEQ_T_CAST(x) static_cast<seq_t>((x))
#else
#   define SEQ_T_CAST(x) ((seq_t)(x))
#endif

seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t cand = 1;   /* candidate integer n being tested */

    while (true) {
        uint64_t p1 = 2 * cand - 1;  /* 2n - 1   */
        uint64_t p2 = 2 * cand + 1;  /* 2n + 1   */

        if (is_prime(p1) && !is_prime(p2)) {
            ++found;
            if (found == n) {
                return SEQ_T_CAST(cand);
            }
        }
        ++cand;
    }
}
