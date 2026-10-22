/*
 * A003847  Order of universal Chevalley group D_8(q), q = prime power.
 * https://oeis.org/A003847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = D(A000961(n+1),8) = q^56*(q^8-1)*Prod_{i=1..7}(q^(2i)-1),
 *            q = A000961(n+1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 1, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t *pp_cache = NULL;
static idx_t pp_size = 0;
static idx_t pp_cap = 0;

static bool is_prime_power(uint64_t n) {
    uint64_t i;
    if (n < 2) return false;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            return n == 1;
        }
    }
    return true;
}

static void ensure_cache(idx_t n) {
    uint64_t i;
    if (pp_size >= n) return;
    if (pp_cap < n) {
        idx_t nc = pp_cap ? pp_cap : 64;
        while (nc < n) nc *= 2;
        pp_cache = (uint64_t *)realloc(pp_cache, (size_t)nc * sizeof(uint64_t));
        pp_cap = nc;
    }
    i = (pp_size > 0) ? pp_cache[pp_size - 1] + 1 : 2;
    while (pp_size < n) {
        if (is_prime_power(i)) pp_cache[pp_size++] = i;
        i++;
    }
}

static seq_t pow_seq(seq_t base, idx_t e) {
    seq_t r = 1;
    while (e > 0) {
        if (e & 1) r *= base;
        e >>= 1;
        if (e) base *= base;
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t q, q2, p, result;
    idx_t i;
    ensure_cache(n);
    /* pp_cache[0] = 2 = A000961(2) */
    q = (seq_t)pp_cache[n - 1];
    result = pow_seq(q, 56);
    result *= pow_seq(q, 8) - 1;
    q2 = q * q;
    p = 1;
    for (i = 1; i <= 7; i++) {
        p *= q2;
        result *= p - 1;
    }
    return result;
}
