/*
 * A183054  h(-24*n+1) where h(d) is the class number of the quadratic field
 *          with discriminant d.
 * https://oeis.org/A183054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(1-24*n), the class number of the quadratic field with
 *            discriminant 1-24*n
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Euclidean GCD for small integers */
static inline int gcd_int(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Computes the class number of a *fundamental* negative discriminant D
   (D < 0, square‑free, D ≡ 1 (mod 4)). */
static seq_t class_number_fundamental(int D) {
    int absD = -D;                    /* |D|, positive */
    int limit = 0;
    while ((long long)3 * (limit + 1) * (limit + 1) <= absD)
        ++limit;

    seq_t count = 0;
    for (int a = 1; a <= limit; ++a) {
        int m4 = 4 * a;
        int Dmod = D % m4;
        if (Dmod < 0) Dmod += m4;

        for (int b0 = 0; b0 < 2 * a; ++b0) {
            int b0sq = (int)((long long)b0 * b0 % m4);
            if (b0sq != Dmod)
                continue;

            int b = (b0 > a) ? b0 - 2 * a : b0;
            int c = (b * b - D) / (4 * a);   /* exact division */

            if (abs(b) > a) continue;
            if (a > c) continue;
            if ((a == c || a == abs(b)) && b < 0) continue;
            if (gcd_int(a, gcd_int(b, c)) != 1) continue;

            ++count;
        }
    }
    return count;
}

/* Prime sieve up to 500 (enough for sqrt(240001)<500). */
static int primes[512];
static size_t primes_cnt;
static bool primes_ready = false;
static void init_primes(void) {
    if (primes_ready) return;
    bool isprime[501];
    for (int i = 2; i <= 500; ++i) isprime[i] = true;
    for (int i = 2; i * i <= 500; ++i) {
        if (isprime[i]) {
            for (int j = i * i; j <= 500; j += i)
                isprime[j] = false;
        }
    }
    for (int i = 2; i <= 500; ++i)
        if (isprime[i]) primes[primes_cnt++] = i;
    primes_ready = true;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined for n=0, but return 0 */

    /* 1 - 24*n --------------------------------------------------------- */
    long long d64 = 1 - 24LL * (long long)n;   /* fits 64 bit for needed n */
    int d = (int)d64;                          /* discriminant (negative) */

    /* reduce d to fundamental discriminant: remove squared factors */
    init_primes();
    int d0 = d;
    if (d0 < 0) {
        int adsor = d0;
        if (adsor < 0) adsor = -adsor;
        for (size_t i = 0; i < primes_cnt &&
                         (long long)primes[i] * primes[i] <= adsor; ++i) {
            int p = primes[i];
            int sq = p * p;
            while (d0 % sq == 0)
                d0 /= sq;
            adsor = d0 < 0 ? -d0 : d0;
        }
    }

    /* compute class number of the fundamental discriminant d0 */
    return class_number_fundamental(d0);
}
