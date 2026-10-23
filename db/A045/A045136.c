/*
 * A045136  Numbers whose base-4 representation contains no 2's and exactly
 *          three 3's.
 * https://oeis.org/A045136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th smallest integer whose base-4 representation
 *            contains no 2’s and exactly three 3’s
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation constructs the number by iterating over possible
   base‑4 digits using combinatorial counts to skip whole groups. */

#include <stdint.h>

/* Pre‑compute binomial coefficients C[n][k] for 0 ≤ n ≤ 20. */
static const uint64_t C[21][21] = {
    {1ULL}, {1ULL, 1ULL},
    {1ULL, 2ULL, 1ULL},
    {1ULL, 3ULL, 3ULL, 1ULL},
    {1ULL, 4ULL, 6ULL, 4ULL, 1ULL},
    {1ULL, 5ULL,10ULL,10ULL, 5ULL, 1ULL},
    {1ULL, 6ULL,15ULL,20ULL,15ULL, 6ULL, 1ULL},
    {1ULL, 7ULL,21ULL,35ULL,35ULL,21ULL, 7ULL, 1ULL},
    {1ULL, 8ULL,28ULL,56ULL,70ULL,56ULL,28ULL, 8ULL, 1ULL},
    {1ULL, 9ULL,36ULL,84ULL,126ULL,126ULL,84ULL,36ULL, 9ULL, 1ULL},
    {1ULL,10ULL,45ULL,120ULL,210ULL,252ULL,210ULL,120ULL,45ULL,10ULL,1ULL},
    {1ULL,11ULL,55ULL,165ULL,330ULL,462ULL,462ULL,330ULL,165ULL,55ULL,11ULL,1ULL},
    {1ULL,12ULL,66ULL,220ULL,495ULL,924ULL,1180ULL,924ULL,495ULL,220ULL,66ULL,12ULL,1ULL},
    {1ULL,13ULL,78ULL,286ULL,715ULL,1287ULL,1716ULL,1716ULL,1287ULL,715ULL,286ULL,78ULL,13ULL,1ULL},
    {1ULL,14ULL,91ULL,364ULL,1001ULL,2002ULL,3003ULL,3432ULL,3003ULL,2002ULL,1001ULL,364ULL,91ULL,14ULL,1ULL},
    {1ULL,15ULL,105ULL,455ULL,1365ULL,3003ULL,5005ULL,6435ULL,5005ULL,3003ULL,1365ULL,455ULL,105ULL,15ULL,1ULL},
    {1ULL,16ULL,120ULL,560ULL,1820ULL,4368ULL,8008ULL,11440ULL,12870ULL,11440ULL,8008ULL,4368ULL,1820ULL,560ULL,120ULL,16ULL,1ULL},
    {1ULL,17ULL,136ULL,680ULL,2380ULL,6188ULL,12376ULL,19448ULL,24310ULL,24310ULL,19448ULL,12376ULL,6188ULL,2380ULL,680ULL,136ULL,17ULL,1ULL},
    {1ULL,18ULL,153ULL,816ULL,3060ULL,8568ULL,18564ULL,31824ULL,43758ULL,48620ULL,43758ULL,31824ULL,18564ULL,8568ULL,3060ULL,816ULL,153ULL,18ULL,1ULL},
    {1ULL,19ULL,171ULL,969ULL,3876ULL,11628ULL,27132ULL,50388ULL,75582ULL,92378ULL,92378ULL,75582ULL,50388ULL,27132ULL,11628ULL,3876ULL,969ULL,171ULL,19ULL,1ULL},
    {1ULL,20ULL,190ULL,1140ULL,4845ULL,15504ULL,38760ULL,77520ULL,125970ULL,167960ULL,184756ULL,167960ULL,125970ULL,77520ULL,38760ULL,15504ULL,4845ULL,1140ULL,190ULL,20ULL,1ULL}
};

/* Powers of two up to 20. */
static const uint64_t POW2[21] = {
    1ULL<<0, 1ULL<<1, 1ULL<<2, 1ULL<<3, 1ULL<<4, 1ULL<<5, 1ULL<<6,
    1ULL<<7, 1ULL<<8, 1ULL<<9, 1ULL<<10, 1ULL<<11, 1ULL<<12,
    1ULL<<13, 1ULL<<14, 1ULL<<15, 1ULL<<16, 1ULL<<17, 1ULL<<18,
    1ULL<<19, 1ULL<<20
};

/* Count valid completions for the remaining 'rem' positions with
   exactly 'need' 3‑digits left to place. */
static inline idx_t count_completions(idx_t rem, idx_t need)
{
    if (need > rem) return 0ULL;
    /* C(rem, need) * 2^(rem - need) */
    return (idx_t)C[rem][need] * (idx_t)POW2[rem - need];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1ULL) return 0;   /* undefined for n < 1, but harmless */

    /* Find the digit length d such that cumulative count >= n. */
    idx_t cumulative = 0ULL;
    idx_t d = 3;  /* minimal number of digits */
    while (1)
    {
        /* total numbers with exactly d digits */
        idx_t total_d = 0ULL;
        /* case 1: most significant digit is 3 */
        if (d >= 3)
        {
            /* choose remaining 2 positions for 3s among last d-1 digits */
            total_d += C[d-1][2] * POW2[d-3];
        }
        /* case 2: most significant digit is 1 */
        if (d >= 4)
        {
            /* choose 3 positions among last d-1 digits for 3s */
            total_d += C[d-1][3] * POW2[d-4];
        }
        if (cumulative + total_d >= n) break;
        cumulative += total_d;
        ++d;
    }

    /* k is the position within d‑digit numbers (1‑based). */
    idx_t k = n - cumulative;

    /* Generate the k-th number in lexicographic order. */
    seq_t val = 0;
    idx_t used3 = 0;

    for (idx_t pos = 0; pos < d; ++pos)
    {
        /* iterate over allowed digits in ascending order */
        int digits[3];
        int nd = 0;
        if (pos == 0)
        {
            digits[nd++] = 1;
            digits[nd++] = 3;
        }
        else
        {
            digits[nd++] = 0;
            digits[nd++] = 1;
            digits[nd++] = 3;
        }

        for (int di = 0; di < nd; ++di)
        {
            int digit = digits[di];
            idx_t new_used3 = used3 + (digit == 3 ? 1 : 0);
            if (new_used3 > 3) continue;

            idx_t rem = d - pos - 1;
            idx_t need = 3 - new_used3;
            idx_t cnt = count_completions(rem, need);

            if (k > cnt)
            {
                k -= cnt;
                continue;
            }

            /* choose this digit */
            val = val * 4 + (seq_t)digit;
            used3 = new_used3;
            break;
        }
    }
    return val;
}
