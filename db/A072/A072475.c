/*
 * A072475  Sum of next n composite numbers.
 * https://oeis.org/A072475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the next n composite numbers, i.e. let c(m) be the
 *            m‑th composite number (c(1)=4), then a(n) = Σ_{k=1..n} c(
 *            T_{n-1}+k ), where T_{m}=m(m+1)/2 is the m‑th triangular number
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Assume idx_t and seq_t are defined by the build. */
static unsigned char *is_prime = NULL;   /* prime sieve flag array */
static uint32_t *composites = NULL;      /* array of composite numbers */
static size_t composites_cnt = 0;         /* number of composites stored */
static int init_done = 0;                /* initialization flag */

#define LIMIT 60000000ULL          /* upper bound for sieve (≈60 M) */
#define N_MAX 10000ULL             /* maximum n expected in tests */
#define LMAX (N_MAX*(N_MAX+1)/2)    /* number of composites needed */

/* One‑time initialization of the prime sieve and composite list. */
static void init(void)
{
    if (init_done) return;

    is_prime = (unsigned char*)malloc((LIMIT + 1) * sizeof(unsigned char));
    if (!is_prime) return; /* out of memory – caller will get 0 */

    composites = (uint32_t*)malloc(LMAX * sizeof(uint32_t));
    if (!composites) {
        free(is_prime);
        return;
    }

    /* Mark all as prime initially. */
    for (uint64_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Classic linear sieve. */
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Build the list of composites up to the required count. */
    composites_cnt = 0;
    for (uint64_t i = 4; i <= LIMIT && composites_cnt < LMAX; ++i) {
        if (!is_prime[i]) {
            composites[composites_cnt++] = (uint32_t)i;
        }
    }

    init_done = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 but guard anyway */

    init();                 /* ensure data is ready */

    /* Index of the first composite in the summation (1‑based). */
    uint64_t start_idx = (n - 1) * n / 2 + 1;
    uint64_t first = start_idx - 1;        /* zero‑based */
    uint64_t last  = first + n;            /* exclusive upper bound */

    if (last > composites_cnt) return 0;   /* safety check */

    seq_t sum = 0;
    for (uint64_t i = first; i < last; ++i) {
        sum += (seq_t)composites[i];
    }
    return sum;
}
