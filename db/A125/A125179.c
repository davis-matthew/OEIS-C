/*
 * A125179  Triangle read by rows: T(n,1) = prime(n) (the n-th prime); T(n,k)
 *          = 0 for k > n; T(n,k) = T(n-1,k) + T(n-1,k-1) for 2 <= k <= n (1
 *          <= k <= n).
 * https://oeis.org/A125179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: triangle T with T(n,1)=prime(n), T(n,k)=T(n-1,k)+T(n-1,k-1). a(n)
   is the nth entry read by rows. */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for primes */
static uint32_t *primes = NULL;
static size_t prime_count = 0;
static bool primes_initialised = false;

/* initialise primes up to a fixed limit sufficient for rows up to ~20000 */
static void init_primes(void)
{
    if (primes_initialised)
        return;
    const size_t limit = 300000;            /* > prime(20000) = 224737 */
    bool *sieve = (bool*)calloc(limit + 1, sizeof(bool));
    if (!sieve)
        exit(EXIT_FAILURE);
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            ++cnt;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    sieve[j] = true;
            }
        }
    }
    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!primes)
        exit(EXIT_FAILURE);
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            primes[idx++] = (uint32_t)i;
    prime_count = cnt;
    free(sieve);
    primes_initialised = true;
}

/* return the nth prime (1‑based) */
static uint32_t nth_prime(idx_t n)
{
    return primes[(size_t)(n - 1)];
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    init_primes();

    idx_t target = n;          /* 1‑based index of element we need */
    idx_t seen   = 0;          /* number of elements processed so far */
    idx_t row    = 0;          /* current row number (1‑based) */

    seq_t *prev = NULL;        /* previous row */
    seq_t *curr = NULL;        /* current row */

    while (seen < target) {
        ++row;                                 /* build next row */
        curr = (seq_t*)malloc((size_t)row * sizeof(seq_t));
        if (!curr)
            exit(EXIT_FAILURE);

        curr[0] = (seq_t)nth_prime(row);      /* first element is prime(row) */

        for (idx_t k = 2; k <= row; ++k) {     /* compute remaining elements */
            seq_t left  = (k - 2 < row - 1) ? prev[k - 2] : (seq_t)0;
            seq_t right = (k - 1 < row - 1) ? prev[k - 1] : (seq_t)0;
            curr[k - 1] = left + right;
        }

        idx_t slice = row;   /* elements added in this row */

        if (seen + slice >= target) {
            idx_t offset = target - seen - 1;   /* 0‑based offset in this row */
            seq_t answer = curr[offset];
            free(prev);
            free(curr);
            return answer;
        }

        seen += slice;
        free(prev);
        prev = curr;
        curr = NULL;
    }

    /* should never be reached */
    free(prev);
    return (seq_t)0;
}
