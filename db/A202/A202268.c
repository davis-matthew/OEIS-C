/*
 * A202268  Numbers in which all digits are neither primes nor zero, i.e., are
 *          members of (1, 4, 6, 8, 9).
 * https://oeis.org/A202268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number whose decimal digits are obtained by mapping each
 *            digit d (0–4) of the base‑5 representation of n–1 (with length
 *            chosen so that the number has that many decimal digits) to
 *            {1,4,6,8,9} according to d→1,4,6,8,9. Positive n (n≥1) return
 *            a(n); n=0 returns 0
 * Range:     u8 n <= 55, u16 n <= 2280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0u) return (seq_t)0;
    /* find the number of decimal digits L:
        total numbers with less than L digits: sum_{k=1}^{L-1}5^k
        we need smallest L with sum+5^L >= n */
    unsigned __int128 pow5 = (unsigned __int128)5;   /* =5^1 */
    unsigned __int128 sum  = 0;                      /* sum of lower powers */
    idx_t L = 1;
    while (sum + pow5 < (unsigned __int128)n) {
        sum += pow5;          /* add 5^L */
        L++;                  /* next length */
        pow5 *= 5;            /* 5^(L+1) */
    }
    /* offset within the L‑digit block */
    unsigned __int128 offset = (unsigned __int128)n - sum - 1u;

    /* mapping from base‑5 digit to allowed decimal digit */
    const seq_t map[5] = { (seq_t)1, (seq_t)4, (seq_t)6, (seq_t)8, (seq_t)9 };

    seq_t result = 0;
    seq_t mult   = 1;                    /* 10^j */
    for (idx_t i = 0; i < L; ++i) {
        unsigned int d = (unsigned int)(offset % 5u);
        offset /= 5u;
        result += map[d] * mult;
        mult *= 10;                      /* next decimal place */
    }

    return result;
}
