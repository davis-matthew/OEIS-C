/*
 * A136742  Product of the n-th run of squarefree numbers.
 * https://oeis.org/A136742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..A120992(n)-1} (A072284(n)+k)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 466, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Run of consecutive squarefree integers: A072284 gives the
   first term, A120992 gives the run length. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool _init_done = false;
static uint32_t *_primes = NULL;
static uint32_t *_prime_squares = NULL;
static size_t _prime_cnt = 0;

/* Initialize primes up to 2000 (covers square roots up to 4,000,000) */
static void _init_primes(void)
{
    const int MAX_P = 2000;
    bool is_prime[MAX_P + 1];
    for (int i = 0; i <= MAX_P; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= MAX_P; ++p) {
        if (is_prime[p]) {
            for (int k = p * p; k <= MAX_P; k += p) is_prime[k] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (int i = 2; i <= MAX_P; ++i) if (is_prime[i]) ++cnt;

    _primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    _prime_squares = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    _prime_cnt = cnt;

    size_t idx = 0;
    for (int i = 2; i <= MAX_P; ++i) {
        if (is_prime[i]) {
            _primes[idx] = (uint32_t)i;
            _prime_squares[idx] = (uint32_t)i * (uint32_t)i;
            ++idx;
        }
    }
    _init_done = true;
}

/* Check if a 32‑bit integer is squarefree by testing prime squares. */
static bool _is_squarefree(uint32_t x)
{
    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint32_t sq = _prime_squares[i];
        if (sq > x) break;
        if (x % sq == 0) return false;
    }
    return true;
}

/* Return the n‑th product of a run of squarefree numbers. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (!_init_done) _init_primes();

    idx_t runs_found = 0;
    uint32_t run_start = 0; /* 0 means no active run */

    for (idx_t num = 1; ; ++num) {
        bool sqfree = _is_squarefree((uint32_t)num);

        if (sqfree) {
            if (run_start == 0) run_start = (uint32_t)num; /* start new run */
            /* continue run */
        } else {
            if (run_start != 0) {
                /* run ended at num-1 */
                ++runs_found;
                if (runs_found == n) {
                    idx_t length = num - run_start;
                    seq_t prod = 1;
                    for (idx_t v = run_start; v < run_start + length; ++v)
                        prod *= (seq_t)v;
                    return prod;
                }
                run_start = 0; /* reset for next run */
            }
        }
    }
}
