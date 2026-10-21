/*
 * A084767  Number of distinct prime-factors of {2*n-p: 2<p<2*n, p prime}.
 * https://oeis.org/A084767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct primes dividing any 2n‑p with p prime
 *            and 2<p<2n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084767: Number of distinct prime-factors of {2*n-p : 2 < p < 2*n, p prime} */

#include <stdbool.h>

/* prime sieve up to 20000 (2*max_n) */
#define MAX_N 20000
static bool initialized = false;
static int primes[2300];
static int prime_count;
static int spf[MAX_N + 1];
static int prime_index[20001];
static unsigned seen_tag[2300];
static unsigned next_tag = 1;

static void init_sieve(void)
{
    int i, j;
    for (i = 0; i <= MAX_N; ++i) {
        spf[i] = 0;
        prime_index[i] = -1;
    }
    prime_count = 0;
    for (i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            primes[prime_count++] = i;
            if ((long long)i * i <= MAX_N) {
                for (j = i * i; j <= MAX_N; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    for (i = 0; i < prime_count; ++i)
        prime_index[primes[i]] = i;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_sieve();
        initialized = true;
    }
    if (n < 1) return 0;

    int two_n = (int)((seq_t)n * 2);
    int limit = two_n - 1;                 /* maximum prime p allowed */
    unsigned this_tag = next_tag++;
    int distinct = 0;

    for (int pi = 0; pi < prime_count && primes[pi] <= limit; ++pi) {
        int p = primes[pi];
        if (p <= 2) continue;              /* skip p = 2 (not >2) */

        int d = two_n - p;
        if (d <= 1) continue;              /* d = 0 or 1 has no prime factors */

        while (d > 1) {
            int factor = spf[d];
            int fi = prime_index[factor];
            if (seen_tag[fi] != this_tag) {
                seen_tag[fi] = this_tag;
                ++distinct;
            }
            while (d % factor == 0) d /= factor;
        }
    }
    return (seq_t)distinct;
}
