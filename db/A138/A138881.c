/*
 * A138881  Array read by rows: row n lists divisors of n-th positive
 *          triangular number A000217(n).
 * https://oeis.org/A138881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 6938, u32 n <= 9452, u64 n <= 9452,
 *            u128 n <= 9452, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A138881 – concatenation of divisors of triangular numbers.
   The sequence is obtained by listing, in increasing order, all divisiors
   of the n-th triangular number Tₙ = n*(n+1)/2, and then concatenating
   the rows for n = 1, 2, 3, ….
   a(1) = 1, a(2) = 1, a(3) = 3, a(4) = 1, a(5) = 2, ...
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static void _init_primes(void);
static void _generate_divisors(size_t idx, seq_t cur, const seq_t *primes,
                               const int *exps, size_t fac_cnt,
                               seq_t *out, size_t *len);
static int _cmp_seq(const void *p1, const void *p2);

/* Small static prime table -------------------------------------------------*/
#define MAX_PRIME 20000
static uint32_t _prime_list[MAX_PRIME + 1];
static size_t _prime_cnt = 0;
static bool _prime_initialized = false;

static void _init_primes(void)
{
    if (_prime_initialized) return;
    bool sieve[MAX_PRIME + 1];
    memset(sieve, true, sizeof(sieve));
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i)
                sieve[j] = false;
        }
    }
    for (int i = 2; i <= MAX_PRIME; ++i)
        if (sieve[i]) _prime_list[_prime_cnt++] = (uint32_t)i;
    _prime_initialized = true;
}

/* Recursive divisor generator ---------------------------------------------*/
static void _generate_divisors(size_t idx, seq_t cur, const seq_t *primes,
                               const int *exps, size_t fac_cnt,
                               seq_t *out, size_t *len)
{
    if (idx == fac_cnt) {
        out[(*len)++] = cur;
        return;
    }
    seq_t p = primes[idx];
    seq_t val = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        _generate_divisors(idx + 1, cur * val, primes, exps,
                           fac_cnt, out, len);
        val *= p;
    }
}

/* Comparator for qsort -----------------------------------------------------*/
static int _cmp_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* --------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* the sequence is offset 1 */
    _init_primes();

    seq_t target = n;
    idx_t row = 1;
    seq_t cumulative = 0;

    while (1) {
        seq_t tri = (seq_t)row * ((seq_t)row + 1) / 2;

        /*--- compute number of divisors of tri ----------------------------*/
        seq_t tmp = tri;
        seq_t div_cnt = 1;

        for (size_t i = 0; i < _prime_cnt; ++i) {
            seq_t p = (seq_t)_prime_list[i];
            if (p * p > tmp) break;
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp > 0) div_cnt *= (seq_t)(exp + 1);
        }
        if (tmp > 1) div_cnt *= 2;   /* remaining prime factor */

        if (cumulative + div_cnt >= target) {
            /* row containing the term found */
            seq_t idx_in_row = target - cumulative - 1;

            /* factorise tri again to build the divisor list */
            seq_t tmp2 = tri;
            seq_t fac_pr[20];
            int  fac_exp[20];
            size_t fac_cnt = 0;

            for (size_t i = 0; i < _prime_cnt && fac_cnt < 20; ++i) {
                seq_t p = (seq_t)_prime_list[i];
                if (p * p > tmp2) break;
                int exp = 0;
                while (tmp2 % p == 0) {
                    tmp2 /= p;
                    ++exp;
                }
                if (exp > 0) {
                    fac_pr[fac_cnt] = p;
                    fac_exp[fac_cnt] = exp;
                    ++fac_cnt;
                }
            }
            if (tmp2 > 1) {
                fac_pr[fac_cnt]  = tmp2;
                fac_exp[fac_cnt] = 1;
                ++fac_cnt;
            }

            /* generate all divisors */
            seq_t divs[2048];
            size_t len = 0;
            _generate_divisors(0, 1, fac_pr, fac_exp, fac_cnt,
                               divs, &len);

            qsort(divs, len, sizeof(seq_t), _cmp_seq);

            return divs[(size_t)idx_in_row];
        }

        cumulative += div_cnt;
        ++row;
    }
}
