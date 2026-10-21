/*
 * A086800  Triangle read by rows in which row n lists differences between
 *          prime(n) and prime(k) for 1 <= k <= n.
 * https://oeis.org/A086800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   a(n) = prime(row(n)) - prime(col(n)) where row(n) =
 *            ceil((sqrt(1+8n)-1)/2), col(n) = n - row(n)*(row(n)-1)/2 (Row
 *            and column indices are 1‑based)
 * Range:     u8 n <= 1540, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static idx_t  *prime_list = NULL;   /* array of primes */
static size_t  prime_cnt   = 0;     /* number of primes stored */
static size_t  prime_cap   = 0;     /* capacity of the array */

/* generate at least 'need' primes (1‑based indexing) */
static void
build_primes_upto(idx_t need)
{
    if (prime_cnt >= need)
        return;                         /* already have enough */

    /* enlarge storage */
    size_t new_cap = prime_cap;
    while (new_cap < need)
        new_cap = new_cap ? new_cap * 2 : 64;  /* minimal initial size */
    if (new_cap > prime_cap) {
        idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!tmp) return;                 /* allocation failure – unlikely in tests */
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    /* choose an upper bound for the nth prime (n >= 6) */
    idx_t limit;
    if (need < 6) {
        limit = 15;
    } else {
        double n = (double)need;
        double est = n * (log(n) + log(log(n)));
        limit = (idx_t)(est) + 10;        /* a small safety margin */
    }

    /* Sieve of Eratosthenes */
    bool *sieve = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!sieve) return;                   /* allocation failure */

    for (size_t i = 0; i <= (size_t)limit; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (sieve[p])
            for (idx_t q = p * p; q <= limit; q += p)
                sieve[q] = false;

    /* collect primes until we have 'need' of them */
    size_t collected = 0;
    for (idx_t i = 2; i <= limit && collected < need; ++i) {
        if (sieve[i]) {
            prime_list[collected++] = i;
        }
    }
    free(sieve);
    prime_cnt = collected;
    /* In the unlikely event that the estimate was too low, retry with a larger bound */
    if (prime_cnt < need) {
        limit = limit * 2 + 10;
        build_primes_upto(need);
    }
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                         /* undefined for n<1, but safe fallback */

    /* find the triangle row r such that r*(r-1)/2 < n <= r*(r+1)/2 */
    long double disc = sqrtl(1.0L + 8.0L * (long double)n);
    idx_t r = (idx_t)((disc - 1.0L) / 2.0L);   /* floor estimate */

    /* adjust up if needed */
    unsigned __int128 t = (unsigned __int128)r * (r + 1) / 2;
    while (t < n) {
        ++r;
        t = (unsigned __int128)r * (r + 1) / 2;
    }
    /* adjust down if overstepped */
    while (r > 0) {
        unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
        if (prev >= n) {
            --r;
        } else {
            break;
        }
    }

    /* column index (1‑based) */
    unsigned __int128 prev_terms = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = (idx_t)(n - (idx_t)prev_terms);

    /* ensure primes up to max(r,c) are available */
    idx_t need = r > c ? r : c;
    build_primes_upto(need);

    seq_t prime_r = (seq_t)prime_list[r - 1];
    seq_t prime_c = (seq_t)prime_list[c - 1];
    return prime_r - prime_c;
}
