/*
 * A066117  Triangle read by rows: T(n,k) = T(n-1,k-1)*T(n,k-1) and T(n,1) =
 *          prime(n).
 * https://oeis.org/A066117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 14, u64 n <= 20, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the (n‑th) element of the triangle read row‑wise defined by
 *
 *   T(1,1) = 2,
 *   T(n,1) = prime(n),
 *   T(n,k) = T(n-1,k-1) * T(n,k-1)        for n>1, k>1 .
 *
 * The sequence a(n) is obtained by flattening the rows of this triangle.
 */

#include <stddef.h>   /* size_t, NULL */
#include <string.h>   /* memset */

/* Maximum row needed for n ≤ 10000 is 142, so a small table suffices. */
#define MAX_R          150
#define MAX_PRIME_CNT  200      /* more than enough for row 142 (prime 727) */
#define PRIME_LIMIT    2000     /* enough to generate the first 200 primes */

/* --- prime table ------------------------------------------------------ */
static seq_t prime_tbl[MAX_PRIME_CNT];
static idx_t prime_cnt = 0;
static int primes_inited = 0;

static void init_primes(void)
{
    if (primes_inited)
        return;

    unsigned char sieve[PRIME_LIMIT + 1];
    memset(sieve, 0, sizeof(sieve));

    idx_t i, j;
    for (i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            if (prime_cnt < MAX_PRIME_CNT)
                prime_tbl[prime_cnt++] = (seq_t)i;

            if (i * i <= PRIME_LIMIT) {
                for (j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = 1;
            }
        }
    }
    primes_inited = 1;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* index 0 is undefined; return 0 as a safe value */

    init_primes();

    /* Find the row r such that r*(r+1)/2 ≥ n  (1‑based).  r stays below 150. */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column within row r (1‑based).  Offset from start of the row. */
    idx_t c = n - r * (r - 1) / 2;

    /* First column – simply the r‑th prime. */
    if (c == 1)
        return prime_tbl[r - 1];

    /* Arrays to hold the previous and current row.  */
    static seq_t prev_row[MAX_R];
    static seq_t curr_row[MAX_R];

    /* Compute rows incrementally up to row r. */
    size_t i;
    for (i = 1; i <= (size_t)r; ++i) {
        /* Fill first element of current row: the i‑th prime. */
        curr_row[0] = prime_tbl[i - 1];

        /* Compute remaining elements using the recurrence. */
        size_t j;
        for (j = 2; j <= i; ++j) {
            curr_row[j - 1] = prev_row[j - 2] * curr_row[j - 2];
        }

        /* If this is the target row, return the requested element. */
        if (i == (size_t)r)
            return curr_row[c - 1];

        /* Copy current row to prev_row for the next iteration. */
        for (j = 0; j < i; ++j)
            prev_row[j] = curr_row[j];
    }

    /* Control should never reach here. */
    return (seq_t)0;
}
