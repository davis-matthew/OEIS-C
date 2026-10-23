/*
 * A022888  n-th prime p(k) such that p(k) + p(k+5) = p(k+1) + p(k+4).
 * https://oeis.org/A022888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th index k (1‑based) such that * p(k) + p(k+5) =
 *            p(k+1) + p(k+4) (p(k) is the k‑th prime)
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *primes_arr = NULL;
static size_t primes_cnt = 0;

static seq_t *validIdx_arr = NULL;
static size_t valid_cnt = 0;

/* Build all primes up to 'limit' (inclusive) and store them in primes_arr.
 * The routine is called only once or after enlarging the limit. */
static void build_primes_limit(idx_t limit)
{
    size_t lim = (size_t)limit;
    /* Allocate a bit array for the sieve */
    bool *composite = (bool*)calloc(lim + 1, sizeof(bool));
    if (!composite) exit(1);

    size_t cnt = 0;
    for (size_t i = 2; i <= lim; ++i) {
        if (!composite[i]) {
            ++cnt;                       /* prime found                */
            if (i * i <= lim) {
                for (size_t j = i * i; j <= lim; j += i)
                    composite[j] = true;
            }
        }
    }

    /* Transfer primes into an array */
    uint64_t *tmp = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!tmp) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (!composite[i]) tmp[idx++] = (uint64_t)i;

    free(composite);
    free(primes_arr);
    primes_arr = tmp;
    primes_cnt = cnt;
}

/* Compute all indices k satisfying p(k)+p(k+5)=p(k+1)+p(k+4).  */
static void compute_valid_indices(void)
{
    /* First count how many there are */
    size_t total = 0;
    for (idx_t k = 1; k + 5 <= (idx_t)primes_cnt; ++k) {
        uint64_t left  = primes_arr[k - 1] + primes_arr[k + 4];
        uint64_t right = primes_arr[k    ] + primes_arr[k + 3];
        if (left == right) ++total;
    }

    seq_t *tmp = (seq_t*)malloc(total * sizeof(seq_t));
    if (!tmp) exit(1);

    /* Record them */
    size_t pos = 0;
    for (idx_t k = 1; k + 5 <= (idx_t)primes_cnt; ++k) {
        uint64_t left  = primes_arr[k - 1] + primes_arr[k + 4];
        uint64_t right = primes_arr[k    ] + primes_arr[k + 3];
        if (left == right) tmp[pos++] = (seq_t)k;
    }

    free(validIdx_arr);
    validIdx_arr = tmp;
    valid_cnt = total;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for out of bounds */

    static bool initialized = false;
    if (!initialized) {
        /* A limit that comfortably covers the first 10 000 terms
           (→ primes up to about 1 000 000). */
        static const idx_t LIMIT = 2000000;
        build_primes_limit(LIMIT);
        compute_valid_indices();
        initialized = true;
    }

    if (n <= (idx_t)valid_cnt) {
        return validIdx_arr[(size_t)(n - 1)];
    }

    /* In the unlikely event that n exceeds the pre‑computed amount,
       extend the sieve until we have enough terms. */
    idx_t new_limit = 2000000;
    while (n > (idx_t)valid_cnt) {
        new_limit *= 2;
        build_primes_limit(new_limit);
        compute_valid_indices();
    }
    return validIdx_arr[(size_t)(n - 1)];
}
