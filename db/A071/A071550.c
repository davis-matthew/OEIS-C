/*
 * A071550  a(n) = (8n)!/n!^8.
 * https://oeis.org/A071550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8n)!/(n!^8)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    static idx_t current_max_n = 0;
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;

    if (n > current_max_n)
    {
        if (prime_list) free(prime_list);
        build_primes_upto(8*n, &prime_list, &prime_cnt);
        current_max_n = n;
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        idx_t e_num = 0, e_den = 0;
        idx_t temp = 8*n;
        while (temp)
        {
            temp /= p;
            e_num += temp;
        }
        temp = n;
        while (temp)
        {
            temp /= p;
            e_den += temp;
        }
        idx_t exp = e_num - 8*e_den;
        if (exp > 0)
            result *= pow_uint(p, exp);
    }

    return result;
}
