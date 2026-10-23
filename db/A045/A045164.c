/*
 * A045164  Numbers whose base-5 representation contains exactly one 0 and
 *          three 1's.
 * https://oeis.org/A045164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th positive integer whose base‑5 representation contains
   exactly one digit 0 and exactly three digits 1.  The implementation
   uses combinatorial counting to locate the correct length and digit
   positions. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Binomial coefficient C(n,k) in idx_t arithmetic.  Assumes 0 <= k <= n. */
static idx_t binom(idx_t n, idx_t k)
{
    if (k > n) return (idx_t)0;
    if (k > n - k) k = n - k;
    idx_t res = (idx_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

/* Power of 3: 3^e.  e is small (≈ length). */
static idx_t pow3(idx_t e)
{
    idx_t r = (idx_t)1;
    for (idx_t i = (idx_t)0; i < e; ++i) r *= (idx_t)3;
    return r;
}

/* Number of valid numbers of length L (L >= 4). */
static idx_t count_len(idx_t L)
{
    if (L < 4) return 0;
    idx_t n = L - (idx_t)1;          /* positions after the leading digit */
    idx_t c = binom(n, (idx_t)3);    /* choose positions for the 3 ones */
    idx_t zero_choices = n;          /* choose position of the single zero */
    idx_t rest = L - (idx_t)4;       /* remaining positions for 2,3,4 */
    return zero_choices * c * pow3(rest);
}

/* Count of completions for the remaining positions. */
static idx_t count_completions(idx_t rem, idx_t z, idx_t o)
{
    if (z + o > rem) return (idx_t)0;
    if (rem == (idx_t)0)
        return ((z == (idx_t)0 && o == (idx_t)0) ? (idx_t)1 : (idx_t)0);

    idx_t c1 = binom(rem, z);
    idx_t c2 = binom(rem - z, o);
    idx_t rest = rem - z - o;
    return c1 * c2 * pow3(rest);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n==0 */

    /* Find the length L of the n-th number. */
    idx_t cumulative = (idx_t)0;
    idx_t L = (idx_t)4;    /* minimal length: 3 ones + 1 zero */
    idx_t cnt = count_len(L);
    while (cumulative + cnt < n) {
        cumulative += cnt;
        ++L;
        cnt = count_len(L);
    }

    /* Zero‑based index within numbers of length L. */
    idx_t k = n - cumulative - (idx_t)1;

    /* Allocate array for digits (uint8_t is sufficient). */
    uint8_t *digits = (uint8_t *)malloc((size_t)L * sizeof(uint8_t));
    if (!digits) return (seq_t)0;   /* allocation failure */

    /* Remaining counts of zeros and ones. */
    idx_t remZ = (idx_t)1;
    idx_t remO = (idx_t)3;

    for (idx_t pos = (idx_t)0; pos < L; ++pos) {
        for (uint8_t d = (uint8_t)0; d <= (uint8_t)4; ++d) {
            if (pos == (idx_t)0 && d == (uint8_t)0) continue;  /* leading 0 forbidden */

            idx_t z = remZ;
            idx_t o = remO;
            if (d == (uint8_t)0) {
                if (z == (idx_t)0) continue;
                --z;
            } else if (d == (uint8_t)1) {
                if (o == (idx_t)0) continue;
                --o;
            }

            idx_t remPos = L - pos - (idx_t)1;
            idx_t cntRem = count_completions(remPos, z, o);

            if (k < cntRem) {
                digits[pos] = d;
                remZ = z;
                remO = o;
                break;
            } else {
                k -= cntRem;
            }
        }
    }

    /* Convert digit array to decimal value. */
    seq_t val = (seq_t)0;
    for (idx_t pos = (idx_t)0; pos < L; ++pos)
        val = val * (seq_t)5 + (seq_t)digits[pos];

    free(digits);
    return val;
}
