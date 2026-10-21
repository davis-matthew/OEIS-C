/*
 * A056275  Number of primitive (aperiodic) word structures of length n using
 *          a 4-ary alphabet.
 * https://oeis.org/A056275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d)*A007581(n/d-1) (OEIS A056275)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* -------------- global static data -------------------- */
static idx_t          precomp_max = 0;           /* largest n pre‑computed   */
static idx_t          *primes      = NULL;       /* prime list              */
static size_t         primes_cnt  = 0;
static bool           *is_comp     = NULL;       /* sieve flag              */
static int            *mu_arr      = NULL;       /* mu values               */
static seq_t          *c_arr       = NULL;       /* c[n] = sum_{k=1}^4 S(n,k)  */
static seq_t         **S_arr       = NULL;       /* S_arr[k][n]             for k=0..4 */

/* -------------- helper prototypes -------------------- */
static void precompute_arrays(idx_t max_n);
static void factorize(idx_t n, idx_t pf[], idx_t exp[], size_t *cnt);

/* -------------- precomputation ----------------------- */
static void precompute_arrays(idx_t max_n)
{
    if (max_n <= precomp_max) return;              /* already ready */

    /* free old data if any */
    if (is_comp)      free(is_comp);
    if (mu_arr)       free(mu_arr);
    if (primes)       free(primes);
    if (c_arr)        free(c_arr);
    if (S_arr) {
        for (size_t i = 0; i < 5; ++i)
            if (S_arr[i]) free(S_arr[i]);
        free(S_arr);
    }

    /* allocate new data */
    is_comp = (bool *)calloc((size_t)(max_n + 1), sizeof(bool));
    mu_arr  = (int  *)malloc((size_t)(max_n + 1) * sizeof(int));
    primes  = (idx_t*)malloc((size_t)(max_n + 1) * sizeof(idx_t));   /* enough space */
    c_arr   = (seq_t*)malloc((size_t)(max_n + 1) * sizeof(seq_t));
    S_arr   = (seq_t**)malloc(5 * sizeof(seq_t*));
    for (size_t k = 0; k < 5; ++k)
        S_arr[k] = (seq_t*)calloc((size_t)(max_n + 1), sizeof(seq_t));

    /* linear sieve for primes and μ */
    mu_arr[0] = 0;
    mu_arr[1] = 1;
    primes_cnt = 0;
    for (idx_t i = 2; i <= max_n; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            mu_arr[i] = -1;
        }
        for (size_t pi = 0; pi < primes_cnt && primes[pi] <= max_n / i; ++pi) {
            idx_t v = i * primes[pi];
            is_comp[v] = true;
            if (i % primes[pi] == 0) {
                mu_arr[v] = 0;
                break;
            } else {
                mu_arr[v] = -mu_arr[i];
            }
        }
    }

    /* Stirling numbers S(n,k) for k=0..4 */
    S_arr[0][0] = 1;                       /* S(0,0)=1 */
    for (idx_t n = 1; n <= max_n; ++n) S_arr[0][n] = 0;
    for (int k = 1; k <= 4; ++k) S_arr[k][0] = 0;

    for (idx_t n = 1; n <= max_n; ++n) {
        for (int k = 1; k <= 4; ++k) {
            S_arr[k][n] = (seq_t)k * S_arr[k][n-1] + S_arr[k-1][n-1];
        }
        c_arr[n] = S_arr[1][n] + S_arr[2][n] + S_arr[3][n] + S_arr[4][n];
    }

    precomp_max = max_n;
}

/* -------------- factorization ------------------------ */
static void factorize(idx_t n, idx_t pf[], idx_t exp[], size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= n; ++i) {
        if (n % primes[i] == 0) {
            idx_t p = primes[i];
            idx_t e = 0;
            do { n /= p; ++e; } while (n % p == 0);
            pf[*cnt] = p;
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        pf[*cnt] = n;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

/* -------------- main sequence function --------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* safety for offset 1 */
    precompute_arrays(n);                     /* ensure tables ready */

    /* factorization of n */
    idx_t pf[16], exp[16];
    size_t cnt;
    factorize(n, pf, exp, &cnt);

    /* iterate over all divisors via explicit stack */
    seq_t result = 0;

    struct Node { idx_t cur; size_t depth; };
    struct Node stack[64];
    size_t sp = 0;
    stack[sp++] = (struct Node){1, 0};

    while (sp > 0) {
        struct Node node = stack[--sp];
        if (node.depth == cnt) {              /* reached a divisor */
            idx_t d = node.cur;
            int mu = mu_arr[d];
            if (mu != 0) {
                idx_t L = n / d;
                seq_t term = c_arr[L];
                result += (mu == 1) ? term : -term;
            }
            continue;
        }

        idx_t p = pf[node.depth];
        idx_t e = exp[node.depth];
        idx_t prod = 1;
        for (idx_t i = 0; i <= e; ++i) {
            stack[sp++] = (struct Node){node.cur * prod, node.depth + 1};
            prod *= p;
        }
    }

    return result;
}
