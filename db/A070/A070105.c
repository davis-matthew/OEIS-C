/*
 * A070105  Number of integer triangles with perimeter n and prime side
 *          lengths which are obtuse and scalene.
 * https://oeis.org/A070105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1246, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of integer triangles with perimeter n, prime side lengths,
   scalene (all sides distinct) and obtuse.  The definition is:
   a(n) = #{ (a,b,c) | a<b<c, a,b,c are primes, a+b+c=n,
                      a+b>c (triangle inequality),
                      c² > a²+b² }.
   For n even, a(n)=0. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_SIEVE 20000           /* enough for n up to about 10000 */

static bool sieve_done = false;
static bool is_prime_arr[MAX_SIEVE + 1];
static uint64_t primes_arr[MAX_SIEVE / 2];   /* enough for primes ≤ MAX_SIEVE */
static size_t primes_cnt = 0;

/* Simple sieve of Eratosthenes, executed once. */
static void init_primes(void)
{
    if (sieve_done) return;
    for (size_t i = 0; i <= MAX_SIEVE; ++i) is_prime_arr[i] = true;
    is_prime_arr[0] = is_prime_arr[1] = false;
    for (size_t i = 2; i * i <= MAX_SIEVE; ++i)
        if (is_prime_arr[i])
            for (size_t j = i * i; j <= MAX_SIEVE; j += i)
                is_prime_arr[j] = false;
    primes_cnt = 0;
    for (size_t i = 2; i <= MAX_SIEVE; ++i)
        if (is_prime_arr[i])
            primes_arr[primes_cnt++] = i;
    sieve_done = true;
}

/* Main entry point demanded by the test harness. */
seq_t a(idx_t n)
{
    init_primes();

    /* For even perimeter the sum of three primes cannot give an obtuse
       scalene triangle (see OEIS comment). */
    if ((n & 1) == 0) return 0;

    seq_t count = 0;
    /* Iterate over all unordered triples of distinct primes (a<b<c). */
    for (size_t ia = 0; ia < primes_cnt; ++ia) {
        uint64_t a_val = primes_arr[ia];
        if (a_val >= n) break;                 /* a alone already ≥ n   */
        for (size_t ib = ia + 1; ib < primes_cnt; ++ib) {
            uint64_t b_val = primes_arr[ib];
            uint64_t sum_ab = a_val + b_val;
            if (sum_ab >= n) break;            /* c would be ≤ 0         */
            uint64_t c_val = n - sum_ab;
            if (c_val <= b_val) continue;      /* enforce a<b<c           */
            if (c_val > MAX_SIEVE) continue;   /* safety for larger n     */
            if (!is_prime_arr[c_val]) continue;/* c must be prime         */

            /* Triangle inequality a+b>c ⇔ 2*(a+b) > n */
            if (sum_ab * 2 <= n) continue;

            /* Obtuse condition: c² > a² + b² */
            uint64_t lhs = c_val * c_val;
            uint64_t rhs = a_val * a_val + b_val * b_val;
            if (lhs <= rhs) continue;

            ++count;
        }
    }
    return count;
}
