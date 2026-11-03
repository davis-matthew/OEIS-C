/*
 * A106557  Largest number that can be obtained by concatenating the two
 *          factors of the n-th semiprime.
 * https://oeis.org/A106557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A084797(A001358(n))
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 915 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A106557: largest number that can be obtained by concatenating the two factors of the n-th semiprime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Construct an array of all primes ≤ limit using a simple sieve. */
static void sieve_primes(int limit, uint32_t **primes_out, size_t *cnt_out)
{
    bool *is_comp = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_comp) { *primes_out = NULL; *cnt_out = 0; return; }

    for (int i = 0; i <= limit; ++i) is_comp[i] = false;

    for (int i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (int j = i * i; j <= limit; j += i)
                is_comp[j] = true;

    size_t cnt = 0;
    for (int i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    uint32_t *pr = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (int i = 2; i <= limit; ++i)
        if (!is_comp[i]) pr[idx++] = (uint32_t)i;

    free(is_comp);
    *primes_out = pr;
    *cnt_out    = cnt;
}

/* Type definitions supplied by the build system:
   idx_t and seq_t are unsigned integer types.  */

/* Configuration: limit for precomputation of semiprimes.
   200000 generates >10 000 semiprimes, sufficient for the intended n range. */
#ifndef MAX_SEM_PRIME
#define MAX_SEM_PRIME 200000
#endif

/* Node holds a semiprime and its two prime factors. */
typedef struct {
    uint32_t val;   /* the semiprime value (≤ MAX_SEM_PRIME) */
    uint32_t p;     /* smaller prime factor */
    uint32_t q;     /* larger prime factor */
} Node;

/* Global storage – all static so initialized once. */
static bool      data_inited = false;
static size_t    sem_count   = 0;
static Node    *sems        = NULL;   /* sorted list of semiprimes */

/* Helper: number of decimal digits for a 32‑bit unsigned integer. */
static int digit_len(uint32_t v)
{
    int len = 0;
    do {
        ++len;
        v /= 10;
    } while (v);
    return len;
}

/* Helper: compute 10^exp as seq_t. */
static seq_t pow10_seq(int exp)
{
    seq_t r = (seq_t)1;
    while (exp--) r *= (seq_t)10;
    return r;
}

/* Compare function for qsort on the Node structure. */
static int compare_nodes(const void *a, const void *b)
{
    const Node *A = (const Node*)a;
    const Node *B = (const Node*)b;
    if (A->val < B->val) return -1;
    if (A->val > B->val) return 1;
    return 0;
}

/* Generate all semiprimes ≤ MAX_SEM_PRIME together with their prime factors.
   The result is sorted by the semiprime value. */
static void generate_semiprimes(void)
{
    uint32_t *primes   = NULL;
    size_t    prime_cnt = 0;

    sieve_primes(MAX_SEM_PRIME, &primes, &prime_cnt);
    if (!primes) return;

    size_t est_cap = prime_cnt * (prime_cnt + 1) / 2 + 10;
    sems = (Node*)malloc(est_cap * sizeof(Node));

    size_t sc = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        for (size_t j = i; j < prime_cnt; ++j) {
            uint32_t q = primes[j];
            uint32_t prod = p * q;
            if (prod > (uint32_t)MAX_SEM_PRIME) break;
            if (sc == est_cap) {
                est_cap *= 2;
                sems = (Node*)realloc(sems, est_cap * sizeof(Node));
            }
            sems[sc].val = prod;
            sems[sc].p   = p;
            sems[sc].q   = q;
            ++sc;
        }
    }
    sem_count = sc;

    qsort(sems, sem_count, sizeof(Node), compare_nodes);

    free(primes);
}

/* Make sure the semiprime table is ready. */
static void init(void)
{
    if (data_inited) return;
    data_inited = true;
    generate_semiprimes();
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1 – return 0 */

    init();

    if ((size_t)n > sem_count) return (seq_t)0;   /* out of pre‑computed range */

    const Node *node   = &sems[n - 1];
    uint32_t    p = node->p;
    uint32_t    q = node->q;

    int len_p = digit_len(p);
    int len_q = digit_len(q);

    seq_t concat1 = ((seq_t)p) * pow10_seq(len_q) + (seq_t)q;
    seq_t concat2 = ((seq_t)q) * pow10_seq(len_p) + (seq_t)p;

    return concat1 > concat2 ? concat1 : concat2;
}
