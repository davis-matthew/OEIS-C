/*
 * A128648  a(n) = denominator(Sum_{k=1..n} (-1)^(k+1)/(prime(k)-1)).
 * https://oeis.org/A128648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator(Sum_{k=1..n} (-1)^(k+1)/(prime(k)-1))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum prime limit for sieving (covers about 17984 primes > 10000 */
#define MAX_PRIME_LIMIT 200000

static bool is_comp[MAX_PRIME_LIMIT + 1];
static uint32_t primes[MAX_PRIME_LIMIT];
static size_t primes_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void) {
    if (primes_initialized) return;
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i) is_comp[i] = false;
    for (size_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i) is_comp[j] = true;
        }
    }
    primes_cnt = 0;
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (primes_cnt >= MAX_PRIME_LIMIT) break;
        }
    }
    primes_initialized = true;
}

/* Euclidean gcd for seq_t (unsigned type). */
static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    init_primes();                 /* ensure primes are available */

    seq_t den = 1;                 /* current denominator (positive) */
    __int128 num = 0;              /* current numerator, signed */

    for (idx_t k = 1; k <= n; ++k) {
        uint32_t pk = primes[k - 1];
        seq_t term_den = (seq_t)pk - 1;           /* pk - 1 > 0 */

        /* sign: + for odd k, - for even k */
        bool positive = (k & 1);

        /* Compute new numerator and denominator */
        __int128 new_num = num * (__int128)term_den
                          + (positive ? (__int128)den : -(__int128)den);
        seq_t new_den = den * term_den;

        /* Reduce the fraction */
        __int128 abs_new_num = new_num >= 0 ? new_num : -new_num;
        seq_t g = gcd_seq((seq_t)abs_new_num, new_den);
        new_num /= (__int128)g;
        new_den /= g;

        /* Store for next iteration */
        num = new_num;
        den = new_den;
    }

    return den; /* denominator in lowest terms */
}
