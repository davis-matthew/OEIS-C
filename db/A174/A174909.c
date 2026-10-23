/*
 * A174909  Triangle T(n,i) whose n-th row gives the number of numbers in any
 *          prime(n)# consecutive numbers whose smallest prime factor is
 *          prime(n-i+1).
 * https://oeis.org/A174909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = element number n when the triangle * defined by T(r,i) is
 *            written row‑by‑row, where * * T(r,1) = ∏_{k=1}^{r-1} (prime(k) –
 *            1) (counts with smallest pf prime(r)) * T(r,i>1) = prime(r) *
 *            T(r-1,i-1) (recurrence) * * The triangle is the number of
 *            integers in any block of length * prime(r)# hav...
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the row r and position pos inside that row.
     * The total number of elements up to row r is r*(r+1)/2. */
    idx_t r = 0;
    idx_t total = 0;
    while (total < n) {
        r++;
        total += r;
    }
    idx_t prev_total = total - r;      /* elements before row r */
    idx_t pos = n - prev_total;        /* 1‑based position in row r (1 ≤ pos ≤ r) */

    /* Precompute small primes and prefix products once. */
    enum { MAX_ROWS = 200 };             /* sufficient for n ≤ 10000 (row ≤ 141) */
    static unsigned int primes[MAX_ROWS];
    static seq_t prefixPrimeProd[MAX_ROWS + 1];
    static seq_t prefixPrimeMinusProd[MAX_ROWS + 1];
    static int initialized = 0;

    if (!initialized) {
        /* Simple prime generator up to the required number of primes. */
        int found = 0;
        unsigned int num = 2;
        while (found < MAX_ROWS) {
            int is_prime = 1;
            for (unsigned int p = 2; (unsigned long long)p * p <= num; ++p) {
                if (num % p == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                primes[found++] = num;
            }
            ++num;
        }

        /* Prefix products of primes and (prime-1) terms. */
        prefixPrimeProd[0] = 1;
        prefixPrimeMinusProd[0] = 1;
        for (int i = 1; i <= MAX_ROWS; ++i) {
            prefixPrimeProd[i] = prefixPrimeProd[i-1] * (seq_t)primes[i-1];
            prefixPrimeMinusProd[i] = prefixPrimeMinusProd[i-1] * ((seq_t)primes[i-1] - 1);
        }
        initialized = 1;
    }

    /* Compute T(r,pos) = (∏_{j=0}^{pos-2} prime(r-j)) * (∏_{k=1}^{r-pos} (prime(k)-1)) */
    /* productP = product of primes from r-pos+2 to r (inclusive) */
    seq_t productP;
    if (pos == 1) {
        productP = 1;
    } else {
        seq_t denom = prefixPrimeProd[r - pos + 1];
        productP = prefixPrimeProd[r] / denom;
    }

    /* productC = product of (prime(k)-1) for k=1 .. r-pos */
    seq_t productC = prefixPrimeMinusProd[r - pos];

    return productP * productC;
}
