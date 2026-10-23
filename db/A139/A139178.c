/*
 * A139178  Numbers n such that n=(pi(d_1)+pi(d_1!)+pi(d_1!!))+...+
 *          (pi(d_k)+pi(d_k!)+pi(d_k!!)) where d_1 d_2 ... d_k is the decimal
 *          expansion of n.
 * https://oeis.org/A139178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A139178:
   a(n) is the nth integer m such that
        m = sum_{digits d of m} ( pi(d) + pi(d!) + pi(d!!) )
   The implementation precomputes the sequence up to its maximum
   possible element (~1.8e5) and answers any query in O(1). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *solutions = NULL;   /* array of found numbers */
static size_t  sol_size   = 0;     /* number of solutions found   */
static size_t  sol_cap    = 0;     /* capacity of the array       */

static bool    initialized = false;

static void grow_solutions(void) {
    if (sol_cap == 0) {
        sol_cap = 1024;
        solutions = (seq_t *)malloc(sol_cap * sizeof(seq_t));
    } else {
        sol_cap *= 2;
        solutions = (seq_t *)realloc(solutions, sol_cap * sizeof(seq_t));
    }
}

static void precompute_sequence(void) {
    /* Max value that can ever satisfy the equation:
       at most 6 digits, each contributing at most f(9) ≈ 29738,
       so max ≈ 6*29738 = 178,428.  Use 200 000 to be safe. */
    const idx_t max_num = 200000;

    /* Pre‑compute prime counting function up to 9! = 362880 */
    const idx_t prime_limit = 362880;
    bool *is_prime = (bool *)malloc((prime_limit + 1) * sizeof(bool));
    uint32_t *pi = (uint32_t *)malloc((prime_limit + 1) * sizeof(uint32_t));
    for (idx_t i = 0; i <= prime_limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= prime_limit; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= prime_limit; q += p)
                is_prime[q] = false;
        }
    }
    pi[0] = 0;
    for (idx_t i = 1; i <= prime_limit; ++i)
        pi[i] = pi[i - 1] + (is_prime[i] ? 1 : 0);
    free(is_prime);

    /* factorials and double factorials for digits 0..9 */
    uint32_t fact[10], dfact[10];
    fact[0] = 1; dfact[0] = 1;
    fact[1] = 1; dfact[1] = 1;
    for (int d = 2; d <= 9; ++d) {
        fact[d] = fact[d - 1] * d;
        if (d % 2 == 0) dfact[d] = d * dfact[d - 2];
        else dfact[d] = d * dfact[d - 2];
    }

    /* f[d] = pi(d) + pi(d!) + pi(d!!) */
    uint32_t f[10];
    for (int d = 0; d <= 9; ++d) {
        f[d] = pi[d] + pi[fact[d]] + pi[dfact[d]];
    }

    /* Find all numbers <= max_num that satisfy the condition */
    for (idx_t m = 1; m <= max_num; ++m) {
        idx_t tmp = m;
        uint32_t sum = 0;
        while (tmp > 0) {
            int d = tmp % 10;
            sum += f[d];
            tmp /= 10;
        }
        if (sum == m) {
            if (sol_size == sol_cap) grow_solutions();
            solutions[sol_size++] = (seq_t)m;
        }
    }
    free(pi);
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) precompute_sequence();

    if (n == 0 || n > sol_size) return (seq_t)0;
    return solutions[n - 1];
}
