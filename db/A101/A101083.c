/*
 * A101083  Largest k such that the product (n+1)(n+2)...(n+k) has at least k
 *          distinct prime factors.
 * https://oeis.org/A101083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A101083:  a(n) = max{k | product_{i=1..k}(n+i) has at least k distinct prime divisors}  */
/*  The algorithm factors each successive number n+k, accumulating distinct primes
    until the count of distinct primes becomes smaller than the current k.   */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>   /* for memset */

#define MAXP 20000        /* sufficient for n ≤ 10⁴ and k up to about 10⁴ */

static bool sieve_done = false;
static int  prime_count = 0;      /* total number of primes ≤ MAXP */
static int  primes[MAXP];
static int  spf[MAXP + 1];        /* smallest prime factor for each number */
static int  prime_index[MAXP + 1] = {0};   /* index of prime p → prime_index[p] */

/* Build prime list and spf table (once) */
static void init_sieve(void)
{
    if (sieve_done) return;

    for (int i = 0; i <= MAXP; ++i) spf[i] = i;

    for (int i = 2; i * i <= MAXP; ++i) {
        if (spf[i] == i) {            /* i is prime */
            for (int j = i * i; j <= MAXP; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    int idx = 0;
    for (int i = 2; i <= MAXP; ++i) {
        if (spf[i] == i) {            /* i is prime */
            primes[idx] = i;
            prime_index[i] = idx;
            ++idx;
        }
    }
    prime_count = idx;               /* number of primes ≤ MAXP */
    sieve_done = true;
}

seq_t a(idx_t n)
{
    init_sieve();                    /* ensure primes and spf ready */

    /* visited[p] tells whether prime primes[p] has already been seen. */
    static bool visited[MAXP];
    memset(visited, 0, (size_t)prime_count);   /* clear only needed entries */

    int distinct = 0;          /* number of distinct primes seen so far */
    int max_k = 0;             /* maximum k satisfying the condition */

    for (int k = 1; ; ++k) {
        uint64_t val = (uint64_t)n + (uint64_t)k;   /* the next number to factor */

        /* factor val using spf; record distinct prime factors */
        uint64_t x = val;
        int last_p = 0;
        while (x > 1) {
            int p = spf[(int)x];
            if (p != last_p) {               /* new distinct prime factor in this number */
                int idxp = prime_index[p];
                if (!visited[idxp]) {
                    visited[idxp] = true;
                    ++distinct;
                }
                last_p = p;
            }
            x /= p;
        }

        if (distinct >= k) {
            max_k = k;                       /* still satisfies the inequality */
        } else {
            break;                           /* further k will also fail */
        }
    }

    return (seq_t)max_k;
}
