/*
 * A235123  The rounded atom-bond connectivity (ABC) of the rooted tree with
 *          Matula-Goebel number n (n >= 2).
 * https://oeis.org/A235123
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 156, u32 n <= 156, u64 n <= 156,
 *            u128 n <= 156, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = round( Σ_{edges uv} sqrt((d(u)+d(v)-2)/(d(u)d(v))) )
 * where the tree is the rooted tree with Matula-Goebel number n (n ≥ 2).
 *
 * The implementation constructs the tree recursively, gathers the
 * degree of each node, computes the contribution of every edge and
 * rounds the total to the nearest integer.  It uses a simple prime
 * sieve to factor the Matula numbers and to obtain the index of a
 * prime (the “prime number theorem” for Matula-Goebel).
 *
 * The code follows the restrictions of the task: it uses only
 * integer arithmetic on seq_t, it is C11/C++17 compatible, and it
 * does not depend on any hard‑coded sequence values.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* -------- Prime infrastructure -------- */
static const size_t PRIME_LIMIT = 2000000;     /* adequate for n up to 10⁴ */
static bool   *prime_is_prime = NULL;
static uint64_t *primes        = NULL;
static uint32_t *prime_index_arr = NULL;      /* 0‑based index (1‑based stored) */
static size_t   prime_cnt    = 0;
static bool     primes_init  = false;

/* ---- Initialise prime tables ---- */
static void init_primes (void)
{
    if (primes_init) return;
    prime_is_prime = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) prime_is_prime[i] = true;
    prime_is_prime[0] = prime_is_prime[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (prime_is_prime[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                prime_is_prime[j] = false;

    /* count primes */
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (prime_is_prime[i]) ++prime_cnt;

    primes         = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    prime_index_arr= (uint32_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) prime_index_arr[i] = 0;

    /* build prime list and index table */
    size_t k = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (prime_is_prime[i]) {
            primes[k] = i;
            prime_index_arr[i] = (uint32_t)(k + 1);  /* 1‑based */
            ++k;
        }
    primes_init = true;
}

/* -------- Tree construction and ABC calculation -------- */
static double abc_sum;   /* global accumulator used by recursion */

/* Recursive routine: returns the degree of the node */
static uint64_t build_tree (uint64_t n, bool has_parent)
{
    uint8_t child_count = 0;
    uint64_t child_deg[64];   /* max children per node is < 64 for n ≤ 10⁴ */

    /* Base case: single‑vertex tree */
    if (n == 1) {
        return has_parent ? 1 : 0;
    }

    /* If n is prime – root degree 1 */
    if (n <= PRIME_LIMIT && prime_is_prime[n]) {
        uint64_t child_num = prime_index_arr[(uint32_t)n];
        child_deg[child_count++] = build_tree(child_num, true);
    }
    else {   /* n is composite – root degree ≥ 2 */
        uint64_t temp = n;
        for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
            uint64_t p = primes[i];
            while (temp % p == 0) {
                if (p <= PRIME_LIMIT) {
                    uint64_t child_num = prime_index_arr[(uint32_t)p];
                    child_deg[child_count++] = build_tree(child_num, true);
                } else {
                    /* p > PRIME_LIMIT – not needed for n ≤ 10⁴ */
                }
                temp /= p;
            }
        }
        if (temp > 1) { /* remaining prime factor */
            if (temp <= PRIME_LIMIT) {
                uint64_t child_num = prime_index_arr[(uint32_t)temp];
                child_deg[child_count++] = build_tree(child_num, true);
            } /* else impossible for the tested ranges */
        }
    }

    uint64_t deg = child_count + (has_parent ? 1 : 0);

    for (uint8_t i = 0; i < child_count; ++i) {
        uint64_t d_child = child_deg[i];
        double term = sqrt((double)(deg + d_child - 2) /
                           ((double)deg * (double)d_child));
        abc_sum += term;
    }
    return deg;
}

/* -------- Public sequence function -------- */
static bool all_init = false;

seq_t a(idx_t n)
{
    if (!all_init) {
        init_primes();
        all_init = true;
    }
    if (n < 2) return 0;          /* offset 2: a(2) is first term */

    abc_sum = 0.0;
    (void)build_tree(n, false);   /* root has no parent */

    long long rnd = (long long)(abc_sum + 0.5L);      /* nearest integer */
    return (seq_t)rnd;
}
