/*
 * A078826  Number of distinct primes contained as binary substrings in binary
 *          representation of n.
 * https://oeis.org/A078826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct primes appearing as contiguous binary
 *            substrings of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: return floor(log2(n)) for n>0, 0 for n==0 */
static unsigned int floor_log2_u64(uint64_t v) {
    unsigned int r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* build a sieve up to MAX_VAL-1 */
#define MAX_VAL 16384   /* maximum value of a binary substring for n <= 10000 (14 bits) */
static bool prime_sieve[MAX_VAL];
static bool sieve_built = false;
static void build_sieve(void) {
    if (sieve_built) return;
    for (int i = 0; i < MAX_VAL; ++i) prime_sieve[i] = true;
    prime_sieve[0] = prime_sieve[1] = false;
    for (int i = 2; i * i < MAX_VAL; ++i) {
        if (prime_sieve[i]) {
            for (int j = i * i; j < MAX_VAL; j += i) prime_sieve[j] = false;
        }
    }
    sieve_built = true;
}

/* timestamped visited array to avoid clearing each call */
static unsigned long long visited[MAX_VAL];
static unsigned long long stamp = 1;
static void timestamp_reset(void) {
    if (stamp == 0) {   /* overflow guard */
        for (int i = 0; i < MAX_VAL; ++i) visited[i] = 0;
        stamp = 1;
    }
    ++stamp;
}

seq_t a(idx_t n)
{
    build_sieve();
    timestamp_reset();

    if (n == 0) return 0;   /* binary "0" has no primes */

    uint64_t num = n;
    unsigned len = floor_log2_u64(num) + 1;   /* number of bits */
    seq_t count = 0;

    /* iterate over all substrings */
    for (unsigned start = 0; start < len; ++start) {
        uint64_t value = 0;
        for (unsigned end = start; end < len; ++end) {
            value = (value << 1) | ((num >> (len - 1 - end)) & 1ULL);
            if (value < MAX_VAL && prime_sieve[value] && visited[value] != stamp) {
                visited[value] = stamp;
                ++count;
            }
        }
    }

    return count;
}
