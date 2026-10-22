/*
 * A003850  Order of simple Chevalley group D_4(q), q = prime power.
 * https://oeis.org/A003850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^12*(q^2-1)*(q^4-1)^2*(q^6-1)/gcd(4,q^4-1), q =
 *            A000961(n+1) (the n-th prime power > 1)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *pp_list = NULL;
static idx_t pp_count = 0;
static uint64_t pp_limit = 0;

static void build_prime_powers(uint64_t limit)
{
    char *comp = (char *)malloc((size_t)limit + 1);
    char *ispp = (char *)malloc((size_t)limit + 1);
    uint64_t i, j;
    idx_t cnt = 0;
    memset(comp, 0, (size_t)limit + 1);
    memset(ispp, 0, (size_t)limit + 1);
    for (i = 2; i <= limit; i++) {
        if (!comp[i]) {
            uint64_t pk;
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
            for (pk = i; pk <= limit; pk *= i) {
                ispp[pk] = 1;
                if (pk > limit / i) break;
            }
        }
    }
    for (i = 2; i <= limit; i++) if (ispp[i]) cnt++;
    free(pp_list);
    pp_list = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(cnt + 1));
    cnt = 0;
    for (i = 2; i <= limit; i++) if (ispp[i]) pp_list[cnt++] = i;
    pp_count = cnt;
    pp_limit = limit;
    free(comp);
    free(ispp);
}

static uint64_t nth_prime_power(idx_t n)
{
    uint64_t lim = pp_limit ? pp_limit : 1024;
    while (pp_count < n) {
        if (pp_limit) lim *= 2;
        build_prime_powers(lim);
    }
    return pp_list[n - 1];
}

seq_t a(idx_t n)
{
    uint64_t qq = nth_prime_power(n);
    seq_t q = (seq_t)qq;
    seq_t q2 = q * q;
    seq_t q4 = q2 * q2;
    seq_t q6 = q4 * q2;
    seq_t f4 = q4 - 1;
    seq_t r = 1;
    int k;
    if (qq % 2 == 1) f4 /= 4;
    r = f4;
    r *= q2 - 1;
    r *= q4 - 1;
    r *= q6 - 1;
    for (k = 0; k < 12; k++) r *= q;
    return r;
}
