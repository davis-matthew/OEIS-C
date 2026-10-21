/*
 * A066575  LCM of numbers m such that 1 <= m <= n, m has a common factor with
 *          n, but m does not divide n.
 * https://oeis.org/A066575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm of all m, 1 ≤ m ≤ n, gcd(m,n) > 1 and m does not
 *            divide n
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 47, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*------ Local helpers and state ------------------------------------------*/
#define MAX_N 10000                     /* maximum n we expect to handle      */
static int spf[MAX_N + 1];             /* smallest prime factor               */
static int primes[2000];
static int prime_cnt = 0;
static bool sieve_done = false;

/* Greatest common divisor for seq_t numbers (Euclidean algorithm). */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Pre‑compute spf[] and primes[] up to MAX_N.                     */
static void init_sieve(void)
{
    int i, j;
    for (i = 0; i <= MAX_N; ++i) spf[i] = i;
    spf[0] = 0; spf[1] = 1;
    for (i = 2; i * i <= MAX_N; ++i)
        if (spf[i] == i)                       /* i is prime */
            for (j = i * i; j <= MAX_N; j += i)
                if (spf[j] == j) spf[j] = i;

    prime_cnt = 0;
    for (i = 2; i <= MAX_N; ++i)
        if (spf[i] == i) primes[prime_cnt++] = i;

    sieve_done = true;
}

/* Return prime divisors of n (without repetition).                */
static void get_prime_divisors(idx_t n, int divs[], int *cnt)
{
    int temp = (int)n;
    *cnt = 0;
    while (temp > 1) {
        int p = spf[temp];
        divs[(*cnt)++] = p;
        while (temp % p == 0) temp /= p;
    }
}

/* Global stamp array for marking processed m values.                 */
static unsigned int stamp[MAX_N + 1] = {0};
static unsigned int current_stamp = 0;   /* incremented for each call      */

/*------ Main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;             /* safeguard for n < 1 */
    if (n == (idx_t)1) return (seq_t)1;

    if (!sieve_done) init_sieve();

    /* Increase stamp, reset if overflowed.                         */
    if (++current_stamp == 0) {                       /* overflow */
        for (int i = 0; i <= MAX_N; ++i) stamp[i] = 0;
        current_stamp = 1;
    }

    /* Prime divisors of n.                                         */
    int divers[16];                     /* 16 suffices for n <= 10^4 */
    int div_cnt;
    get_prime_divisors(n, divers, &div_cnt);

    seq_t lcm_val = (seq_t)1;           /* lcm accumulator            */

    for (int i = 0; i < div_cnt; ++i) {
        int p = divers[i];
        for (int m = p; m <= (int)n; m += p) {          /* multiples of p */
            if (n % m != 0) { /* m does not divide n */
                if (stamp[m] != current_stamp) {
                    stamp[m] = current_stamp;
                    seq_t g = seq_gcd(lcm_val, (seq_t)m);
                    lcm_val = (lcm_val / g) * (seq_t)m;
                }
            }
        }
    }
    return lcm_val;
}
