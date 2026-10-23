/*
 * A136260  Primes which contain the digit 2 or the digit 3.
 * https://oeis.org/A136260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes that contain digit 2 or 3 in their decimal representation */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static seq_t *qual_primes = NULL;      /* array of qualifying primes */
static idx_t qual_count   = 0;          /* number of primes currently stored */
static idx_t current_limit = 0;         /* maximum number up to which we have sieved */

/* check if a prime contains digit 2 or 3 */
static bool has23(seq_t n) {
    seq_t x = n;
    while (x > 0) {
        uint64_t d = (uint64_t)(x % 10);
        if (d == 2 || d == 3) return true;
        x /= 10;
    }
    return false;
}

/* simple integer square root using floating point, safe for idx_t up to 64 bits */
static idx_t isqrt_idx(idx_t n) {
    idx_t r = (idx_t)(sqrt((double)n) + 0.5);
    while (r * r > n) --r;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* generate all primes up to 'limit' and keep those containing 2 or 3 */
static void generate_upto(idx_t limit) {
    /* allocate composite flags */
    unsigned char *composite = (unsigned char *)malloc(limit + 1);
    if (!composite) exit(1);
    memset(composite, 0, limit + 1);

    /* sieve */
    idx_t sqrtL = isqrt_idx(limit);
    for (idx_t i = 2; i <= sqrtL; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                composite[j] = 1;
            }
        }
    }

    /* count total primes to allocate enough space for all primes before filtering */
    idx_t total_primes = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!composite[i]) ++total_primes;

    seq_t *temp = (seq_t *)malloc(sizeof(seq_t) * total_primes);
    if (!temp) exit(1);

    /* filter primes that contain 2 or 3 */
    idx_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!composite[i] && has23((seq_t)i)) {
            temp[pos++] = (seq_t)i;
        }
    }

    /* replace old cache */
    free(qual_primes);
    qual_primes = temp;
    qual_count = pos;
    current_limit = limit;

    free(composite);
}

/* ensure that we have at least 'n' qualifying primes stored */
static void ensure_cache(idx_t n) {
    if (qual_count >= n) return;

    idx_t limit = (current_limit > 100) ? current_limit : 100;
    while (qual_count < n) {
        limit *= 2;               /* grow the limit */
        generate_upto(limit);     /* regenerate the cache */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but guard */
    ensure_cache(n);
    return qual_primes[n - 1];
}
