/*
 * A032447  Inverse function of phi( ).
 * https://oeis.org/A032447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, look, tabf
 * Formula:   a(n) = the n‑th integer in the list obtained by sorting all
 *            positive integers by (phi(m), m) in increasing order
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  The limit up to which φ is pre‑computed.
    For n ≤ 10000 this value is more than enough.              */
#define LIMIT 200000UL

static seq_t *phi_arr = NULL;        /* φ(0..LIMIT)        */
static idx_t *sorted_idx = NULL;     /* indices 1..LIMIT   */
static idx_t *primes = NULL;         /* primes ≤ LIMIT     */
static size_t primes_cnt = 0;        /* number of primes   */
static bool initialized = false;    /* initialization flag */

/* Comparator for qsort: compare by φ, then by the integer itself. */
static int cmp_phi(const void *a, const void *b)
{
    const idx_t i = *(const idx_t *)a;
    const idx_t j = *(const idx_t *)b;
    seq_t pi = phi_arr[i];
    seq_t pj = phi_arr[j];
    if (pi < pj) return -1;
    if (pi > pj) return 1;
    return (i < j) ? -1 : ((i > j) ? 1 : 0);
}

/* One‑time initialization: compute φ[1..LIMIT] with a linear sieve
   and fill the sorted index array. */
static void init(void)
{
    if (initialized) return;

    phi_arr    = (seq_t *)calloc(LIMIT + 1, sizeof(seq_t));
    sorted_idx = (idx_t *)calloc(LIMIT,      sizeof(idx_t));
    primes     = (idx_t *)malloc(LIMIT * sizeof(idx_t));   /* ample space */

    /* --- φ via linear sieve --------------------------------------- */
    phi_arr[1] = 1;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (phi_arr[i] == 0) {          /* i is prime */
            phi_arr[i] = (seq_t)i - 1;
            primes[primes_cnt++] = i;
        }
        for (size_t k = 0; k < primes_cnt; ++k) {
            idx_t p = primes[k];
            idx_t ip = i * p;
            if (ip > LIMIT) break;
            if (i % p == 0) {
                phi_arr[ip] = phi_arr[i] * (seq_t)p;
                break;
            } else {
                phi_arr[ip] = phi_arr[i] * ((seq_t)p - 1);
            }
        }
    }

    /* --- fill sorted index array ----------------------------------- */
    for (idx_t i = 1; i <= LIMIT; ++i)
        sorted_idx[i - 1] = i;

    qsort(sorted_idx, LIMIT, sizeof(idx_t), cmp_phi);

    initialized = true;
}

/* Public entry point.  For n < 1 the function returns 0,
   but the offset is 1 so a(1) is the first produced term.            */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init();   /* ensure data is ready */

    if (n > LIMIT) {
        /* In the unlikely event that n exceeds LIMIT, grow the tables.
           The current implementation does not support this case.       */
        return 0;
    }

    return (seq_t)sorted_idx[n - 1];
}
