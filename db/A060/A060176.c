/*
 * A060176  Square array A(n,k) = the largest power of k-th prime which
 *          divides n, read by falling antidiagonals.
 * https://oeis.org/A060176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = the largest power of the k-th prime dividing row r, where
 *            (r,k) is obtained by reading the square array
 *            A(r,k)=p_k^{v_{p_k}(r)} by falling antidiagonals. Equivalent
 *            mapping: let s satisfy s*(s+1)/2 ≥ n, let offset = n -
 *            (s-1)*s/2, then row = offset, col = s-offset+1
 * Range:     u8 n <= 9259, u16 n <= 9259, u32 n <= 9259, u64 n <= 9259,
 *            u128 n <= 9259, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned primes[2000];
static int primes_cnt = 0;

static void init_primes(void)
{
    if (primes_cnt > 0) return;                        /* already done */

    const int limit = 20000;
    unsigned char sieve[limit + 1];
    for (int i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (int i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (int j = i * i; j <= limit; j += i)
                sieve[j] = 0;

    int cnt = 0;
    for (int i = 2; i <= limit && cnt < 2000; ++i)
        if (sieve[i]) primes[cnt++] = (unsigned)i;
    primes_cnt = cnt;
}

static unsigned get_prime(unsigned k)
{
    init_primes();
    if (k == 0 || k > (unsigned)primes_cnt) return 1;   /* fallback */
    return primes[k - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                               /* out of offset */

    unsigned long long N   = (unsigned long long)n;
    unsigned long long s   = 1;                         /* diagonal index */
    while (s * (s + 1) / 2 < N) ++s;                    /* find s */

    unsigned long long prev    = (s - 1) * s / 2;          /* before s */
    unsigned long long offset  = N - prev;                /* 1‑based on diag */
    unsigned long long row     = offset;                  /* n-coordinate */
    unsigned long long col     = s - offset + 1;          /* prime index   */

    unsigned p   = get_prime((unsigned)col);              /* kth prime     */
    unsigned long long temp = row;
    seq_t result = 1;

    while (temp % p == 0) {
        temp /= p;
        result *= (seq_t)p;
    }
    return result;
}
