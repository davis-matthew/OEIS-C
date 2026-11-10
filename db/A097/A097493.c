/*
 * A097493  Primes which are two greater than A097492 terms.
 * https://oeis.org/A097493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) is the n-th prime of the form (Product_{k=1..m}
 *            A006512(k)) + 2, found by multiplying successive greater twin
 *            primes and testing each running product + 2 for primality
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 7 terms through n = 7 (2026-11-07)
 * Helpers:   is_prime_u64, mem, mulmod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod_seq.h"

static uint32_t A097493_small[400];
static int A097493_nsmall = 0;

static void A097493_init_small(void)
{
    uint32_t q;
    if (A097493_nsmall) return;
    for (q = 3; q < 3000 && A097493_nsmall < 400; q += 2)
        if (is_prime_u64(q)) A097493_small[A097493_nsmall++] = q;
}

static bool A097493_is_big_type(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 130; i++) t *= 2;
    return t != 0;
}

static seq_t A097493_powmod(seq_t b, seq_t e, seq_t m)
{
    seq_t r = 1;
    b = b % m;
    while (e > 0) {
        if (e % 2 == 1) r = mulmod_seq(r, b, m);
        e /= 2;
        if (e > 0) b = mulmod_seq(b, b, m);
    }
    return r;
}

/* Miller-Rabin for odd N larger than 2^64 */
static bool A097493_mr_large(seq_t N)
{
    static const int bases[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    seq_t d, nm1 = N - 1;
    int s = 0, i, j;
    for (i = 0; i < A097493_nsmall; i++)
        if (N % (seq_t)A097493_small[i] == 0) return false;
    d = nm1;
    while (d % 2 == 0) { d /= 2; s++; }
    for (i = 0; i < 16; i++) {
        seq_t x = A097493_powmod((seq_t)bases[i], d, N);
        bool ok;
        if (x == 1 || x == nm1) continue;
        ok = false;
        for (j = 1; j < s; j++) {
            x = mulmod_seq(x, x, N);
            if (x == nm1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static bool A097493_isprime(seq_t N)
{
    seq_t lim64 = (seq_t)UINT64_MAX;
    if (N <= lim64) return is_prime_u64((uint64_t)N);
    return A097493_mr_large(N);
}

seq_t a(idx_t n)
{
    static bool inited = false;
    static bool big;
    static seq_t *found = 0;
    static idx_t nfound = 0, cap = 0;
    static seq_t prod;
    static uint64_t p;
    static bool overflow = false;

    if (!inited) {
        A097493_init_small();
        big = A097493_is_big_type();
        prod = 1;
        p = 5;
        cap = 16;
        found = mem_new(seq_t, cap);
        inited = true;
    }

    while (nfound < n) {
        if (overflow) return 0;
        if (is_prime_u64(p) && is_prime_u64(p - 2)) {
            seq_t np = prod * (seq_t)p;
            if (!big && np / (seq_t)p != prod) { overflow = true; return 0; }
            prod = np;
            if (!big && prod + 2 < prod) { overflow = true; return 0; }
            if (A097493_isprime(prod + 2)) {
                if (nfound == cap) {
                    cap *= 2;
                    found = mem_resize(seq_t, found, cap);
                }
                found[nfound++] = prod + 2;
            }
        }
        p += 2;
    }
    return found[n - 1];
}
