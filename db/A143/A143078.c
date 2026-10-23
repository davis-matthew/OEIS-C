/*
 * A143078  Triangle read by rows: row n (n >= 2) has length pi(n) (see
 *          A000720) and the k-th term gives the exponent of prime(k) in the
 *          prime factorization of n.
 * https://oeis.org/A143078
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = exponent of prime(k) in the factorisation of n where k is
 *            the position of the prime in the list of primes up to n. The
 *            sequence is read row‑wise from the triangle of exponents
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* The following static state is used to pre‑compute primes,
   their count π(i), and the cumulative number of entries up to each row. */
static bool _seq_a_inited = false;
static const idx_t _seq_a_limit = 200000;          /* pre‑compute up to this many integers */

static idx_t   *_seq_a_pi_arr  = NULL;          /* π(i) = number of primes ≤ i   */
static idx_t   *_seq_a_cum_arr = NULL;          /* cumulative row sizes          */
static uint8_t *_seq_a_is_prime = NULL;         /* boolean sieve                */
static uint32_t *_seq_a_primes = NULL;          /* list of primes               */
static size_t  _seq_a_prime_count = 0;

/* --- initialization ---------------------------------------------------- */
static void _seq_a_initialize(void)
{
    if (_seq_a_inited) return;
    const size_t limit = _seq_a_limit;

    /* Allocate arrays */
    _seq_a_is_prime = (uint8_t *)calloc(limit + 1, 1);
    _seq_a_pi_arr   = (idx_t   *)calloc(limit + 1, sizeof(idx_t));
    _seq_a_cum_arr  = (idx_t   *)calloc(limit + 1, sizeof(idx_t));
    /* Rough upper bound for number of primes: ≈ limit / log(limit) < limit/10 for our range */
    _seq_a_primes   = (uint32_t*)malloc((limit / 10 + 200) * sizeof(uint32_t));

    /* Mark all >1 as potentially prime */
    for (size_t i = 2; i <= limit; ++i) _seq_a_is_prime[i] = 1;
    _seq_a_is_prime[0] = 0;
    _seq_a_is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (size_t p = 2; p * p <= limit; ++p) {
        if (_seq_a_is_prime[p]) {
            for (size_t q = p * p; q <= limit; q += p)
                _seq_a_is_prime[q] = 0;
        }
    }

    /* Gather primes and π array */
    size_t pc = 0;
    idx_t cumulative = 0;
    _seq_a_pi_arr[0] = 0;
    _seq_a_pi_arr[1] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (_seq_a_is_prime[i]) {
            _seq_a_primes[pc++] = (uint32_t)i;
        }
        _seq_a_pi_arr[i] = (idx_t)pc;
    }
    _seq_a_prime_count = pc;

    /* Build cumulative array of row lengths */
    _seq_a_cum_arr[0] = 0;
    _seq_a_cum_arr[1] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        cumulative += _seq_a_pi_arr[i];
        _seq_a_cum_arr[i] = cumulative;
    }

    _seq_a_inited = true;
}

/* ----- main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* below offset */

    _seq_a_initialize();                 /* ensure tables are ready */

    /* Transform to a 1‑based index for the cumulative array. */
    idx_t idx = n - 1;                    /* idx ∈ [1,  ∞) */

    /* Binary search for the row that contains idx. */
    idx_t lo = 2;
    idx_t hi = _seq_a_limit;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (_seq_a_cum_arr[mid] >= idx)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;                      /* row number (≥ 2) */

    /* Zero‑based position within that row */
    idx_t pos = idx - _seq_a_cum_arr[row - 1] - 1;

    /* Corresponding prime */
    uint32_t p = _seq_a_primes[pos];      /* pos < π(row) */

    /* Exponent of p in factorisation of row */
    idx_t exponent = 0;
    idx_t temp = row;
    while (temp % p == 0) {
        ++exponent;
        temp /= p;
    }

    return (seq_t)exponent;
}
