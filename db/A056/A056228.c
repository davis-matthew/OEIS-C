/*
 * A056228  Image of Euler totient function (A000010) under "little Hankel"
 *          transform that sends [c_0, c_1, ...] to [d_0, d_1, ...] where d_n
 *          = c_n^2 - c_{n+1}*c_{n-1}.
 * https://oeis.org/A056228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = φ(n+1)^2 - φ(n+2)*φ(n) (Little Hankel transform of
 *            Euler’s totient)
 * Range:     i8 n <= 15, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Prime sieve for the small range needed for φ(n) with n ≤ 10 000. */
static int primes[200];
static int prime_cnt = 0;
static bool primes_init = false;

static void init_primes(void)
{
    bool comp[201] = {false};
    for (int i = 2; i <= 200; ++i) {
        if (!comp[i]) {
            primes[prime_cnt++] = i;
            if ((long long)i * i <= 200) {
                for (int j = i * i; j <= 200; j += i)
                    comp[j] = true;
            }
        }
    }
}

/* Compute φ(m) with trial division using the above primes. */
static seq_t phi(idx_t m)
{
    if (!primes_init) {
        init_primes();
        primes_init = true;
    }
    seq_t result = m;
    idx_t temp = m;

    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        if ( (seq_t)p * (seq_t)p > temp )
            break;
        if ( temp % (idx_t)p == 0 ) {
            while ( temp % (idx_t)p == 0 )
                temp /= (idx_t)p;
            result -= result / (seq_t)p;
        }
    }
    if (temp > 1)
        result -= result / (seq_t)temp;
    return result;
}

seq_t a(idx_t n)
{
    seq_t phi_n  = phi(n);
    seq_t phi_n1 = phi(n + 1);
    seq_t phi_n2 = phi(n + 2);
    return phi_n1 * phi_n1 - phi_n2 * phi_n;
}
