/*
 * A051076  Primes p such that x^14 = -2 has a solution mod p.
 * https://oeis.org/A051076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that x^14 = -2 has a solution mod
 *            p. The condition is equivalent for an odd prime p to
 *            (-2)^((p-1)/gcd(14,p-1)) ≡ 1 (mod p). The prime 2 always
 *            satisfies
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/* ---------- small helper routines ---------- */

static uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = mulmod64(result, base, mod);
        base = mulmod64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* ---------- prime generation routine ---------- */
struct PrimeState {
    uint64_t *list;          /* list of found primes */
    size_t   sz;             /* number of primes stored */
    size_t   cap;            /* allocated capacity */
    uint64_t cand;           /* next candidate to test */
};

static struct PrimeState ps = {NULL, 0, 0, 2};

static void ensure_prime_cap(size_t need)
{
    if (ps.cap >= need) return;
    size_t new_cap = ps.cap ? ps.cap * 2 : 1024;
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(ps.list, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1); /* allocation failure */
    ps.list = tmp;
    ps.cap  = new_cap;
}

static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < ps.sz; ++i) {
        uint64_t p = ps.list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

static uint64_t next_prime(void)
{
    while (1) {
        uint64_t n = ps.cand;
        ps.cand += (n == 2 ? 1 : 2); /* after 2 skip even numbers */
        if (is_prime_candidate(n)) {
            ensure_prime_cap(ps.sz + 1);
            ps.list[ps.sz++] = n;
            return n;
        }
    }
}

/* ---------- good prime cache ---------- */
struct GoodState {
    uint64_t *good;          /* list of primes satisfying the condition */
    size_t   sz;             /* number stored */
    size_t   cap;            /* allocated capacity */
};

static struct GoodState gs = {NULL, 0, 0};

static void ensure_good_cap(size_t need)
{
    if (gs.cap >= need) return;
    size_t new_cap = gs.cap ? gs.cap * 2 : 1024;
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(gs.good, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    gs.good = tmp;
    gs.cap  = new_cap;
}

static void fill_good_up_to(size_t target)
{
    while (gs.sz < target) {
        uint64_t p = next_prime();           /* primes in ascending order */
        bool ok = false;
        if (p == 2ULL) {
            ok = true;
        } else {
            uint64_t g = gcd64(14ULL, p - 1ULL);
            uint64_t e = (p - 1ULL) / g;
            if (powmod64(p - 2ULL, e, p) == 1ULL)
                ok = true;
        }
        if (ok) {
            ensure_good_cap(gs.sz + 1);
            gs.good[gs.sz++] = p;
        }
    }
}

/* ---------- exported function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, return 0 */

    /* ensure we have at least n good primes */
    fill_good_up_to((size_t)n);

    return (seq_t)gs.good[n - 1];
}
