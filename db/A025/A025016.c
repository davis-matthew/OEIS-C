/*
 * A025016  Final digits of !n = Sum_{i=0..n} i! (A003422) for very large n,
 *          read from right.
 * https://oeis.org/A025016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = ( Σ_{k=0}^{∞} k! ) / 10^n (mod 10) * i.e. the n‑th
 *            (0‑based) digit of the 10‑adic sum of all factorials
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/*  Compute digit a(n) of the 10‑adic sum Σ k! (A025016).                */
/*  The routine works for any idx_t and seq_t defined by the build.      */
/*  It keeps only the lowest n+1 decimal digits in a small base‑10^4    */
/*  representation, so the memory use is O(n) and the running time is   */
/*  O(n * n) for n up to the required limits (≈10000).                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* number of decimal digits we must keep: positions 0..n          */
    const idx_t keep_digits = n + 1;
    /* base 10^4 representation for efficient arithmetic            */
    const uint32_t BASE = 10000;          /* 10^4 */
    const uint32_t POW10[4] = {1, 10, 100, 1000};

    /* number of limbs needed to store keep_digits decimal digits   */
    const size_t limbs = (keep_digits + 3) / 4;   /* ceil(keep_digits/4) */

    uint32_t *fact = (uint32_t *)calloc(limbs, sizeof(uint32_t));
    uint32_t *sum  = (uint32_t *)calloc(limbs, sizeof(uint32_t));
    if (!fact || !sum) { /* allocation failure – return 0 */
        free(fact); free(sum);
        return (seq_t)0;
    }

    /* 0! = 1 ------------------------------------------------------- */
    fact[0] = 1;
    sum[0]  = 1;                     /* add 0! to the sum */

    /* iterate over k = 1 .. while k! can affect position n             */
    uint64_t cumV5 = 0;              /* cumulative number of factors 5 in k!   */
    for (idx_t k = 1; ; ++k) {
        /* compute the extra 5‑factors contributed by k                 */
        idx_t tmp = k;
        uint64_t inc = 0;
        while (tmp % 5 == 0) {
            ++inc;
            tmp /= 5;
        }
        cumV5 += inc;
        if (cumV5 > n) break;       /* further factorials would be 0 mod 10^{n+1} */

        /* multiply current factorial by k  (mod 10^{keep_digits})      */
        uint64_t carry = 0;
        for (size_t l = 0; l < limbs; ++l) {
            uint64_t prod = (uint64_t)fact[l] * k + carry;
            fact[l] = (uint32_t)(prod % BASE);
            carry   = prod / BASE;
        }

        /* add the new factorial to the running sum (mod 10^{keep_digits})*/
        carry = 0;
        for (size_t l = 0; l < limbs; ++l) {
            uint64_t sumv = (uint64_t)sum[l] + fact[l] + carry;
            sum[l] = (uint32_t)(sumv % BASE);
            carry  = sumv / BASE;
        }
    }

    /* extract the digit at position n (0‑based least significant)     */
    size_t limb_idx  = (size_t)(n / 4);
    size_t offset    = (size_t)(n % 4);
    uint32_t limb    = sum[limb_idx];
    uint32_t digit   = (limb / POW10[offset]) % 10;

    free(fact);
    free(sum);
    return (seq_t)digit;
}
