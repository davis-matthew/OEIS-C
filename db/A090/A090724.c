/*
 * A090724  Defined in Comments lines.
 * https://oeis.org/A090724
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A090724
    a(n) for n ≥ 4 is obtained from the nth prime (starting with 2)
    by:  taking its last decimal digit d ∈ {1,3,7,9},
    mapping d -> {3,6,5,4} respectively (1→3, 3→6, 7→5, 9→4),
    and then subtracting (n−4) mod 4 + 1 (mod 4) from that mapped value.
    The result lies in {0,1,2,3,4,5,6}.  This implementation uses a
    simple sieve to generate primes on demand and memoises the prime list
    so that repeated calls are efficient.  It runs in O(n) time for n up
    to ~10000.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------  prime sieve and memoisation --------------------- */
static uint64_t *primes   = NULL;   /* array of generated primes     */
static size_t   primeCnt = 0;      /* number of primes in the array */

static void sieve_limit(size_t limit)
{
    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, 1);
    if (!is_comp) return;  /* out of memory – abort silently */

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    /* allocate and fill prime array */
    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(is_comp);
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) new_primes[idx++] = i;

    free(is_comp);
    free(primes);
    primes    = new_primes;
    primeCnt  = cnt;
}

static void ensure_primes(size_t required)
{
    if (primeCnt >= required) return;           /* already enough */

    /* rough upper bound for the n‑th prime: p_n < n*(log n + log log n)
       but a linear overshoot suffices for the n ≤ 10 000 range. */
    size_t estimate = required * 20 + 15;
    while (1) {
        sieve_limit(estimate);
        if (primeCnt >= required) break;
        estimate *= 2;                          /* double the limit */
    }
}

/* --------------  main sequence function -------------------------- */
seq_t a(idx_t n)
{
    /* a(n) is defined only for n ≥ 4; values below the offset are
       unspecified – return 0 for safety. */
    if (n < 4) return 0;

    /* first obtain the nth prime (primes[0] = 2). */
    idx_t p_index = n - 1;
    ensure_primes((size_t)p_index + 1);         /* at least that many primes */

    uint64_t prime = primes[p_index];
    uint8_t last_digit = (uint8_t)(prime % 10);

    /* mapping: 1→3, 3→6, 7→5, 9→4, other digits never occur. */
    static const uint8_t map_digit[10] = {
        0, 3, 0, 6, 0, 0, 0, 5, 0, 4
    };
    uint8_t mapped = map_digit[last_digit];

    /* offset = ((n‑4) mod 4 + 1) mod 4  (pattern 1,2,3,0). */
    uint8_t offset = (uint8_t)((((n - 4) % 4) + 1) % 4);
    uint8_t result = mapped - offset;          /* guaranteed to stay ≥0 */

    return (seq_t)result;
}
