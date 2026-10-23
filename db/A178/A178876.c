/*
 * A178876  a(n) = number of 7-digit primes with digit sum n, where n runs
 *          through the non-multiples of 3 in the range [2..62].
 * https://oeis.org/A178876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = number of 7‑digit primes whose digit sum equals the n‑th
 *            non‑multiple of 3 between 2 and 62 (OEIS offset 1)
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* pre‑computed list of the 41 non‑multiples of 3 in [2,62] */
static const idx_t digit_sums[41] = {
    2,4,5,7,8,10,11,13,14,16,17,19,20,22,23,25,26,28,29,31,32,
    34,35,37,38,40,41,43,44,46,47,49,50,52,53,55,56,58,59,61,62
};

/* counts[sum] will hold the number of 7‑digit primes with digit sum 'sum' */
static seq_t counts[63];

/* initialization flag */
static bool ready = false;

/* --------------------------------------------------------------------- */
/*  Helper to initialise the counts                                  */
/* --------------------------------------------------------------------- */
static void init_counts(void)
{
    const idx_t LIMIT = 9999999;     /* largest 7‑digit number */
    const idx_t START = 1000000;     /* smallest 7‑digit number */

    /*  Allocate an array of bytes to indicate prime status */
    unsigned char *is_prime = (unsigned char *)calloc(LIMIT + 1, 1);
    if (!is_prime) {                  /* allocation failure – leave ready = false */
        ready = false;
        return;
    }

    /*  Sieve of Eratosthenes  */
    for (idx_t i = 2; i <= LIMIT; ++i)
        is_prime[i] = 1;

    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= LIMIT; j += p)
                is_prime[j] = 0;
        }
    }

    /*  Count digit sums for all 7‑digit primes  */
    for (idx_t n = START; n <= LIMIT; ++n) {
        if (!is_prime[n])
            continue;

        unsigned sum = 0;
        idx_t x = n;
        while (x) {
            sum += (unsigned)(x % 10);
            x    /= 10;
        }
        if (sum <= 62 && sum % 3 != 0)
            counts[sum]++;          /* seq_t is unsigned, ++ is allowed */
    }

    free(is_prime);
    ready = true;
}

/* --------------------------------------------------------------------- */
/*  Main interface: a(n)                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* below offset – undefined */

    if (!ready)
        init_counts();

    if (!ready)                         /* still failed to initialise  */
        return 0;

    /*  There are only 41 admissible sums; n>41 → 0  */
    if (n > 41)
        return 0;

    idx_t sum = digit_sums[n - 1];
    return counts[sum];
}
