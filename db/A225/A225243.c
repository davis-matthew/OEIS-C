/*
 * A225243  Irregular triangle read by rows, where row n contains the distinct
 *          primes that are contained in the binary representation of n as
 *          substrings; first row = [1] by convention.
 * https://oeis.org/A225243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1156, u16 n <= 6218, u32 n <= 6218, u64 n <= 6218,
 *            u128 n <= 6218, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth element of an irregular triangle that is read
   row‒by‒row.  Row k (k ≥ 2) contains the distinct primes found as
   binary substrings of k; row 1 contains the single element 1.
   The function generates rows until the required index is reached. */

/* ---- universal headers ----------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---- small‑prime sieve (up to 65535) ------------------------------------- */
static bool small_prime[65536];
static bool small_prime_inited = false;
static int  prime_list[7000];     /* primes ≤ 65535, for trial division */
static size_t prime_list_cnt = 0;

static void init_small_prime(void)
{
    if (small_prime_inited) return;
    small_prime_inited = true;

    /* build sieve */
    for (int i = 0; i < 65536; ++i) small_prime[i] = true;
    small_prime[0] = small_prime[1] = false;
    for (int i = 2; (long long)i * i < 65536; ++i)
        if (small_prime[i])
            for (int j = i * i; j < 65536; j += i)
                small_prime[j] = false;

    /* build prime list up to 65535 for trial division */
    prime_list_cnt = 0;
    for (int i = 2; i < 65536; ++i)
        if (small_prime[i])
            prime_list[prime_list_cnt++] = i;
}

/* ---- primality test for values that may exceed 65535 --------------------- */
static bool is_prime_value(uint64_t v)
{
    if (v < 65536) return small_prime[(uint32_t)v];
    /* trial division using primes up to sqrt(v) */
    for (size_t i = 0; i < prime_list_cnt; ++i)
    {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > v) break;
        if (v % p == 0) return false;
    }
    return true;
}

/* ---- main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    /* n is 1‑based.  Count terms as rows are generated. */
    idx_t count = 0;
    init_small_prime();

    for (idx_t row = 1; ; ++row)
    {
        /* Row 1 is by convention [1] */
        if (row == 1)
        {
            ++count;
            if (count == n) return (seq_t)1;
            continue;
        }

        /* Determine binary representation of 'row' */
        uint64_t x = row;
        int bits[64];
        int len = 0;
        while (x) { bits[len++] = (int)(x & 1ULL); x >>= 1; }
        /* bits array now holds least significant bit first.
           We will process substrings by reading from start to end,
           which corresponds to most significant to least significant. */
        /* Reverse bits to most significant first for easier handling */
        for (int i = 0; i < len / 2; ++i)
        {
            int tmp = bits[i];
            bits[i] = bits[len - 1 - i];
            bits[len - 1 - i] = tmp;
        }

        /* Collect primes for this row */
        uint32_t primes_arr[512];
        size_t primes_cnt = 0;

        for (int start = 0; start < len; ++start)
        {
            uint32_t val = 0;
            for (int end = start; end < len; ++end)
            {
                val = (val << 1) | (uint32_t)bits[end];
                if (val < 2) continue;
                if (is_prime_value(val))
                {
                    primes_arr[primes_cnt++] = val;
                    if (primes_cnt == 512) /* safety guard */
                        break;
                }
            }
            if (primes_cnt == 512) break;
        }

        if (primes_cnt == 0) continue;

        /* Sort ascending */
        for (size_t i = 1; i < primes_cnt; ++i)
        {
            uint32_t key = primes_arr[i];
            size_t j = i;
            while (j > 0 && primes_arr[j - 1] > key)
            {
                primes_arr[j] = primes_arr[j - 1];
                --j;
            }
            primes_arr[j] = key;
        }

        /* Remove duplicates */
        size_t uniq = 0;
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            if (uniq == 0 || primes_arr[i] != primes_arr[i - 1])
                primes_arr[uniq++] = primes_arr[i];
        }
        primes_cnt = uniq;

        /* Walk through terms in this row */
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            ++count;
            if (count == n) return (seq_t)primes_arr[i];
        }
    }

    /* unreachable */
    return (seq_t)0;
}
