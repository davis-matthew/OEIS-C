/*
 * A097490  Primes which are two greater than A097489 terms.
 * https://oeis.org/A097490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime of the form A097489(k) + 2, where A097489(k)
 *            is the product of the first k lesser twin primes (A001359);
 *            found by search, with trial division and then Miller-Rabin
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static bool A097490_detect_big(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 130; i++)
        t = t * 2;
    return t != 0;
}

static seq_t A097490_addm(seq_t x, seq_t y, seq_t m)
{
    seq_t gap = m - y;
    if (x >= gap)
        return x - gap;
    return x + y;
}

static seq_t A097490_mulm(seq_t x, seq_t y, seq_t m, bool big)
{
    if (big)
        return (x * y) % m;
    {
        seq_t r = 0;
        x = x % m;
        while (y > 0) {
            if ((y & 1) != 0)
                r = A097490_addm(r, x, m);
            x = A097490_addm(x, x, m);
            y >>= 1;
        }
        return r;
    }
}

static seq_t A097490_powm(seq_t b, seq_t e, seq_t m, bool big)
{
    seq_t r = 1;
    b = b % m;
    while (e > 0) {
        if ((e & 1) != 0)
            r = A097490_mulm(r, b, m, big);
        b = A097490_mulm(b, b, m, big);
        e >>= 1;
    }
    return r;
}

static bool A097490_probable_prime(seq_t v, bool big)
{
    static const int bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53};
    uint64_t q;
    seq_t d, vm1;
    int s = 0, i, j;
    if (v < 2)
        return false;
    for (q = 2; q < 2000; q++) {
        if (!is_prime_u64(q))
            continue;
        if (v == (seq_t)q)
            return true;
        if (v % (seq_t)q == 0)
            return false;
    }
    vm1 = v - 1;
    d = vm1;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    for (i = 0; i < (int)(sizeof(bases) / sizeof(bases[0])); i++) {
        seq_t x = A097490_powm((seq_t)bases[i], d, v, big);
        bool ok = false;
        if (x == 1 || x == vm1)
            continue;
        for (j = 1; j < s; j++) {
            x = A097490_mulm(x, x, v, big);
            if (x == vm1) {
                ok = true;
                break;
            }
        }
        if (!ok)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    bool big = A097490_detect_big();
    seq_t prod = 1;
    idx_t found = 0;
    uint64_t p;
    for (p = 3; ; p += 2) {
        seq_t sp, nxt, cand;
        if (!(is_prime_u64(p) && is_prime_u64(p + 2)))
            continue;
        sp = (seq_t)p;
        if (sp == 0)
            return 0;
        nxt = prod * sp;
        if (!big && nxt / sp != prod)
            return 0; /* overflow */
        prod = nxt;
        cand = prod + 2;
        if (!big && cand < prod)
            return 0; /* overflow */
        if (A097490_probable_prime(cand, big)) {
            ++found;
            if (found == n)
                return cand;
        }
    }
}
