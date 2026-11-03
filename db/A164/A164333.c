/*
 * A164333  Primes prime(k) such that all integers in the interval
 *          [(prime(k-1)+1)/2, (prime(k)-1)/2] are composite numbers.
 * https://oeis.org/A164333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164333: primes p_k where all integers in 
   [(p_{k-1}+1)/2 , (p_k-1)/2] are composite. */

#include <stdint.h>
#include <stdbool.h>

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static inline uint64_t next_prime_u64(uint64_t n)
{
    uint64_t cand = n + 1;
    while (!is_prime_big((uint64_t)cand))
        ++cand;
    return cand;
}

static inline bool is_prime_u64(uint64_t n)
{
    return is_prime_big((uint64_t)n);
}

static inline uint64_t first_prime_in_interval(uint64_t start, uint64_t end)
{
    uint64_t x = start;
    while (x <= end && !is_prime_u64(x))
        ++x;
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t count = 0;
    uint64_t last = 2;          /* previous prime considered */
    while (true) {
        uint64_t cur = next_prime_u64(last);  /* next prime after 'last' */
        if (cur < 5) {          /* skip 2 and 3 */
            last = cur;
            continue;
        }
        /* interval [(last+1)/2 , (cur-1)/2] */
        uint64_t L = (last + 1) / 2;
        uint64_t R = (cur - 1) / 2;
        uint64_t first = first_prime_in_interval(L, R);
        if (first > R) {        /* no prime inside the interval */
            ++count;
            if (count == n)
                return (seq_t)cur;
        }
        last = cur;
    }
}
