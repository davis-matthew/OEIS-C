/*
 * A001041  a(0)=12; thereafter a(n) = 12 times the product of the first n
 *          primes.
 * https://oeis.org/A001041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 12; a(n) = 12 * prime(1)*prime(2)*...*prime(n) = 12 *
 *            A002110(n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *a1041_primes = 0;
static idx_t a1041_count = 0;
static idx_t a1041_cap = 0;

static void a1041_ensure(idx_t need) {
    uint64_t cand;
    if (a1041_count >= need) return;
    if (a1041_cap < need) {
        idx_t nc = a1041_cap ? a1041_cap : 16;
        while (nc < need) nc *= 2;
        a1041_primes = (uint64_t *)realloc(a1041_primes, (size_t)nc * sizeof(uint64_t));
        a1041_cap = nc;
    }
    if (a1041_count == 0) {
        a1041_primes[0] = 2;
        a1041_count = 1;
    }
    cand = a1041_primes[a1041_count - 1] + 1;
    while (a1041_count < need) {
        int ok = 1;
        idx_t j;
        for (j = 0; j < a1041_count; j++) {
            uint64_t p = a1041_primes[j];
            if (p * p > cand) break;
            if (cand % p == 0) { ok = 0; break; }
        }
        if (ok) a1041_primes[a1041_count++] = cand;
        cand++;
    }
}

seq_t a(idx_t n) {
    seq_t result = (seq_t)12;
    idx_t i;
    if (n == 0) return result;
    a1041_ensure(n);
    for (i = 0; i < n; i++) {
        result *= (seq_t)a1041_primes[i];
    }
    return result;
}
