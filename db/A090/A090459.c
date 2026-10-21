/*
 * A090459  Duplicate of A090417.
 * https://oeis.org/A090459
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the (n+2)-th prime (for n ≥ 2)
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* static storage for generated primes */
static uint32_t *prime_list = NULL;  /* array of primes, 0‑based (prime_list[0] = 2) */
static size_t prime_count = 0;       /* number of primes stored */

/* ensure that prime_list contains at least 'needed' primes (needed = n+2) */
static void ensure_primes_upto(idx_t needed)
{
    if (prime_count >= needed) return;          /* already have enough */

    /* estimate upper bound for the needed‑th prime using p_n < n (ln n + ln ln n) */
    double d = (double)needed;
    uint32_t bound;
    if (needed <= 6) {
        bound = 15;          /* small hard‑coded bound to cover first 6 primes */
    } else {
        bound = (uint32_t)(d * (log(d) + log(log(d))) + 3.0) + 1;
    }

    /* allocate sieve */
    uint8_t *sieve = (uint8_t *)calloc(bound + 1, sizeof(*sieve));
    if (!sieve) return;           /* memory failure – give up */

    /* create new prime array of the required size */
    uint32_t *new_list = (uint32_t *)malloc(needed * sizeof(*new_list));
    if (!new_list) {
        free(sieve);
        return;
    }

    size_t count = 0;
    for (uint32_t i = 2; i <= bound; ++i) {
        if (!sieve[i]) {
            if (count < needed) {
                new_list[count++] = i;
                if (count == needed) break;   /* enough primes, can stop searching */
            }
            /* mark multiples if i² <= bound */
            if ((uint64_t)i * (uint64_t)i <= bound) {
                for (uint64_t j = (uint64_t)i * i; j <= bound; j += i) {
                    sieve[(size_t)j] = 1;
                }
            }
        }
    }
    free(sieve);

    if (count < needed) {        /* safety check – should not happen */
        free(new_list);
        return;
    }

    /* replace old list with new list */
    free(prime_list);
    prime_list = new_list;
    prime_count = count;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n < 2 – return 0 */
    idx_t idx = n + 2;             /* we need the (n+2)-th prime */
    ensure_primes_upto(idx);
    /* prime_list is zero‑based: prime_list[0] = 2 */
    return (seq_t)prime_list[idx - 1];
}
