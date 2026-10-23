/*
 * A184156  The Wiener polarity index of the rooted tree with Matula-Goebel
 *          number n.
 * https://oeis.org/A184156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) – Wiener polarity index of the rooted tree with Matula‑Goebel
 * number n.  The value is computed recursively from the prime factorisation
 * of n as described in the OEIS entry.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/*------------------------------------------------------------
 * Primes up to MAX_PRIME are generated once and stored together
 * with their indices in the prime sequence (starting at 1).
 *------------------------------------------------------------*/

#define MAX_PRIME 10000

static uint32_t primes[MAX_PRIME / 10 + 1];
static size_t primes_count = 0;

static uint16_t prime_to_index[MAX_PRIME + 1] = {0};

static bool primes_initialized = false;

static void init_primes(void)
{
    /* Sieve of Eratosthenes – static array, no variable-length */
    static bool composite[MAX_PRIME + 1] = {false};
    for (idx_t i = 2; (uint64_t)i * i <= MAX_PRIME; ++i)
        if (!composite[i])
            for (idx_t j = i * i; j <= MAX_PRIME; j += i)
                composite[j] = true;

    for (idx_t i = 2; i <= MAX_PRIME; ++i)
        if (!composite[i]) {
            primes[primes_count] = (uint32_t)i;
            prime_to_index[i] = (uint16_t)(primes_count + 1);  /* indices start at 1 */
            ++primes_count;
        }
}

/* Ensure the primes table is available before it is needed */
static void ensure_primes(void)
{
    if (!primes_initialized) { init_primes(); primes_initialized = true; }
}

/*------------------------------------------------------------
 * Recursive solver.
 * For each node we compute:
 *    cnt1 – number of children (distance‑1 nodes from this root)
 *    cnt2 – number of grandchildren (distance‑2 nodes from this root)
 *    pairs – Wiener polarity index inside the subtree rooted here
 *------------------------------------------------------------*/

typedef struct {
    seq_t cnt1;
    seq_t cnt2;
    seq_t pairs;
} Node;

static Node solve(idx_t n)
{
    if (n == 1)                     /* leaf */
        return (Node){0, 0, 0};

    seq_t child_cnt  = 0;            /* number of child subtrees */
    seq_t sum_pairs  = 0;            /* sum of pairs inside children */
    seq_t sum_cnt1   = 0;            /* total grandchildren count in root */
    seq_t sum_cnt2   = 0;            /* total nodes at distance 3 from root */

    idx_t temp = n;

    for (size_t i = 0; i < primes_count &&
             (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        while (temp % p == 0) {                 /* each factor gives one child */
            idx_t child_num = prime_to_index[p];   /* child Matula number */
            Node child = solve(child_num);

            ++child_cnt;
            sum_pairs  += child.pairs;
            sum_cnt1   += child.cnt1;
            sum_cnt2   += child.cnt2;
            temp /= p;
        }
    }

    if (temp > 1) {                                /* remaining prime factor */
        idx_t child_num = prime_to_index[temp];
        Node child = solve(child_num);

        ++child_cnt;
        sum_pairs  += child.pairs;
        sum_cnt1   += child.cnt1;
        sum_cnt2   += child.cnt2;
    }

    seq_t cnt1   = child_cnt;          /* number of children (distance‑1 nodes) */
    seq_t cnt2   = sum_cnt1;           /* nodes at distance‑2 from this root */
    seq_t pairs  = sum_pairs +            /* inside subtrees */
                  (cnt1 > 0 ? (cnt1 - 1) * cnt2 : 0) + /* child–grandchild pairs */
                  sum_cnt2;                 /* root‑distance‑3 pairs */

    return (Node){cnt1, cnt2, pairs};
}

/*------------------------------------------------------------
 * Public interface required by the build system.
 *------------------------------------------------------------*/

seq_t a(idx_t n)
{
    ensure_primes();                   /* make sure primes are ready */
    return solve(n).pairs;
}
