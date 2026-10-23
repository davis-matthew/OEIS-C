/*
 * A133162  Trajectory of 1 under the morphism 1 -> {1,1,2,1}, 2 -> {2}.
 * https://oeis.org/A133162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements A133162 using the block‑recursion definition:
        S_0 = 1
        S_{t+1} = S_t S_t 2 S_t
   The function reduces n until it reaches 1 or a power of 3. */
#include <stddef.h>   /* size_t */

/* The caller supplies the 3 integer types. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;      /* a(1) = 1 */

    /* Powers of 3 up to 3⁴³ – more than enough for 64‑bit indices. */
    static const idx_t POW3[] = {
        1ULL, 3ULL, 9ULL, 27ULL, 81ULL, 243ULL,
        729ULL, 2187ULL, 6561ULL, 19683ULL, 59049ULL,
        177147ULL, 531441ULL, 1594323ULL, 4782969ULL,
        14348907ULL, 43046721ULL, 129140163ULL, 387420489ULL,
        1162261467ULL, 3486784401ULL, 10460353203ULL,
        31381059609ULL, 94143178827ULL, 282429536481ULL,
        847288609443ULL, 2541865828329ULL, 7625597484987ULL,
        22876792454961ULL, 68630377364883ULL, 205891132094649ULL,
        617673396283947ULL, 1853020188851841ULL, 5559060566555523ULL,
        16677181699666569ULL, 50031545098999707ULL,
        150094635296999821ULL, 450283905890999463ULL,
        1350851717672991409ULL, 4052555153018974227ULL,
        12157665459056928801ULL
    };
    const size_t POW3_CNT = sizeof(POW3) / sizeof(POW3[0]);

    /* Reduce n until it becomes 1 or a power of 3. */
    while (n != 1) {
        /* Find t such that (3^t‑1)/2 < n ≤ (3^(t+1)‑1)/2. */
        size_t t = 0;
        while (t + 1 < POW3_CNT && n > (POW3[t + 1] - 1) / 2)
            ++t;

        idx_t pow_t = POW3[t];
        idx_t p_t   = (pow_t - 1) / 2;

        if (n == pow_t)               /* n = 3^t */
            return (seq_t)2;

        if (n < pow_t)
            n -= p_t;                  /* n' = n – p_t   */
        else
            n -= pow_t;                /* n' = n – pow_t (2p_t +1 = pow_t) */
    }

    return (seq_t)1;
}
