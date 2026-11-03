/*
 * A235138  a(n) = Sum_{k=1..n} k^phi(n) (mod n) where phi(n) = A000010(n).
 * https://oeis.org/A235138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=1..n} k^phi(n) (mod n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9852 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using phi(n) supplied by calc_phi, and modular exponentiation.   */
#include <stdint.h>
#include "../utils/totient_seq.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* offset starts at 1 */
    /* phi(n) fits in uint64_t for n <= 2^64-1 */
    seq_t phi_seq = totient_seq((seq_t)n);
    uint64_t phi = (uint64_t)phi_seq;
    uint64_t mod = (uint64_t)n;
    uint64_t sum = 0;

    for (uint64_t k = 1; k <= mod; ++k) {
        uint64_t base = k % mod;
        uint64_t exp = phi;
        uint64_t res = 1 % mod;
        /* binary exponentiation modulo n */
        while (exp > 0) {
            if (exp & 1ull) res = (res * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        sum += res;
        /* keep sum within 64‑bit bounds; mod is small so this is safe */
        if (sum >= (1ull << 63)) sum %= mod;
    }
    return (seq_t)(sum % mod);
}
