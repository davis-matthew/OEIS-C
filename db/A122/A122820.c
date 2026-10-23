/*
 * A122820  Array read by rows in which n-th row contains n successive primes
 *          with least sum divisible by n.
 * https://oeis.org/A122820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth entry of the triangle where the r‑th row contains the
   first sequence of r consecutive primes whose sum is divisible by r
   (row read by rows).  */
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* external integer types supplied by the build system */
typedef uint64_t idx_t;   /* index type */

/* ---------------------------- prime handling ---------------------------- */
static idx_t *primes = NULL;
static size_t primes_size = 0, primes_cap = 0;
static seq_t *prefix = NULL;          /* prefix[i] = sum of primes[0..i-1] */
static size_t prefix_size = 0, prefix_cap = 0;
static idx_t last_checked = 2;

/* grow and readjust the prime array */
static void grow_primes(void) {
    if (primes_cap == 0) primes_cap = 16;
    else                primes_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp;
}

/* grow and readjust the prefix sum array */
static void grow_prefix(void) {
    if (prefix_cap == 0) prefix_cap = 32;
    else                prefix_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prefix, prefix_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    prefix = tmp;
}

/* add a prime to the tables */
static void add_prime(idx_t p) {
    if (primes_size >= primes_cap) grow_primes();
    primes[primes_size++] = p;

    if (prefix_size >= prefix_cap) grow_prefix();
    seq_t new_sum = prefix[prefix_size - 1] + (seq_t)p;
    prefix[prefix_size++] = new_sum;
}

/* simple primality test using known primes */
static void gen_next_prime(void) {
    idx_t cand = last_checked + 1;
    for (;;) {
        if (cand < 3) cand = 3;
        else if ((cand & 1ULL) == 0) cand++;

        uint32_t lim = (uint32_t)sqrt((double)cand);
        bool prime = true;
        for (size_t i = 0; i < primes_size && primes[i] <= lim; ++i) {
            if (cand % primes[i] == 0) { prime = false; break; }
        }
        if (prime) {
            add_prime(cand);
            last_checked = cand;
            return;
        }
        ++cand;
    }
}

/* ---------------------------- row start cache -------------------------- */
static idx_t *row_start = NULL;        /* row_start[r] = index in primes of first element of row r (1‑based) */
static size_t row_start_size = 0, row_start_cap = 0;
static idx_t max_row = 0;

/* grow the row_start array */
static void grow_row_start(void) {
    if (row_start_cap == 0) row_start_cap = 16;
    else                    row_start_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(row_start, row_start_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    row_start = tmp;
}

/* make sure we have cached all rows up to R */
static void ensure_rows(idx_t R) {
    while (max_row < R) {
        ++max_row;
        idx_t r = max_row;

        /* generate enough primes to test possible windows */
        while (primes_size < r) gen_next_prime();

        bool found = false;
        for (;;) {
            size_t m = primes_size;
            for (size_t k = 0; k + r <= m; ++k) {
                seq_t sum = prefix[k + r] - prefix[k];
                if ((sum % (seq_t)r) == 0) {
                    if (row_start_size >= row_start_cap) grow_row_start();
                    row_start[row_start_size++] = (idx_t)k;
                    found = true;
                    break;
                }
            }
            if (found) break;
            gen_next_prime();  /* need more primes */
        }
    }
}

/* ---------------------------- initialisation --------------------------- */
static void init(void) {
    static bool inited = false;
    if (inited) return;
    inited = true;

    /* primes and prefix */
    primes_cap = 16;
    primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    if (!primes) exit(EXIT_FAILURE);

    prefix_cap = 32;
    prefix = (seq_t *)malloc(prefix_cap * sizeof(seq_t));
    if (!prefix) exit(EXIT_FAILURE);
    prefix[0] = (seq_t)0;
    prefix_size = 1;

    /* row_start */
    row_start_cap = 16;
    row_start = (idx_t *)malloc(row_start_cap * sizeof(idx_t));
    if (!row_start) exit(EXIT_FAILURE);
    row_start[0] = 0;  /* dummy for 1‑based indexing */
    row_start_size = 1;

    /* first prime */
    add_prime((idx_t)2);
    last_checked = 2;
}

/* ---------------------------- public function -------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for offset 1 */

    init();

    /* find the row r containing the nth term (rows read consecutively) */
    idx_t r = (idx_t)((sqrt((double)8.0 * (double)n + 1.0) - 1.0) / 2.0);
    if ((r * (r + 1)) / 2 < n) ++r;

    idx_t prev_total = (r - 1) * r / 2;
    idx_t pos = n - prev_total - 1;  /* 0‑based within the row */

    ensure_rows(r);

    idx_t prime_index = row_start[r] + pos;
    if (prime_index >= primes_size) return (seq_t)0;  /* safety */

    return (seq_t)primes[prime_index];
}
