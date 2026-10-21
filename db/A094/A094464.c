/*
 * A094464  Least initial value for a Euclid/Mullin sequence whose 3rd term (=
 *          least prime divisor of 1+2p) equals the n-th prime. prime(1)=2 is
 *          never a third term, so offset=2.
 * https://oeis.org/A094464
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the smallest prime S such that the third term
   of the Euclid/Mullin sequence starting with S equals the n‑th prime
   (prime(1)=2).  For S>2 the second term is 2, so the third term is
   lpf(1+2S).  The algorithm tests primes S in ascending order until
   this lpf equals the required prime.  */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)           /* sequence is undefined below offset 2 */
        return 0;

    seq_t target = (seq_t)get_prime(n);   /* the n-th prime */

    for (idx_t idx = 1; ; ++idx)          /* iterate over primes S */
    {
        seq_t S = (seq_t)get_prime(idx);
        seq_t a2 = (S == 2) ? (seq_t)3 : (seq_t)2;

        /* Compute T = 1 + S * a2  (third term's argument) */
        seq_t T = S * a2 + (seq_t)1;

        /* Quick rejection if target is not even a divisor of T */
        if (T % target != 0)
            continue;

        /* Ensure no smaller prime divides T */
        bool ok = true;
        for (idx_t j = 1; ; ++j)
        {
            seq_t p = (seq_t)get_prime(j);
            if (p >= target)    /* all primes ≥ target tested */
                break;
            if (T % p == 0)
            {
                ok = false;
                break;
            }
        }

        if (ok)   /* third term equals the target prime */
            return S;
    }
}
