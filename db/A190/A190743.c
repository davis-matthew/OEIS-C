/*
 * A190743  a(n) = n + [nr/v] + [ns/v] + [nt/v] + [nu/v] + [nw/v], where
 *          r=sin(x), s=cos(x), t=tan(x), u=csc(x), v=sec(x), w=cot(x),
 *          x=Pi/8.
 * https://oeis.org/A190743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n*sin(π/8)/sec(π/8)] + [n*cos(π/8)/sec(π/8)] +
 *            [n*tan(π/8)/sec(π/8)] + [n*csc(π/8)/sec(π/8)] +
 *            [n*cot(π/8)/sec(π/8)]
 * Range:     u8 n <= 35, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t DEN = 1000000000ULL;       /* 10⁹  */

static const uint64_t NUM1 = 353553390ULL;       /* √2/4                      */
static const uint64_t NUM2 = 853553390ULL;       /* (2+√2)/4                  */
static const uint64_t NUM3 = 382683432ULL;       /* √(2-√2)/2                 */
static const uint64_t NUM4 = 2414213562ULL;      /* 1+√2                      */
static const uint64_t NUM5 = 2230443158ULL;      /* (2+√2)/(2√(2-√2)) ≃ 2.230443158 */

static inline seq_t floor_mul_div(idx_t n, uint64_t num)
{
    uint64_t q = n / DEN;
    uint64_t r = n % DEN;
    seq_t part1 = (seq_t)q * (seq_t)num;
    unsigned __int128 prod = (unsigned __int128)r * num;
    seq_t part2 = (seq_t)(prod / DEN);
    return part1 + part2;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;
    result += floor_mul_div(n, NUM1);
    result += floor_mul_div(n, NUM2);
    result += floor_mul_div(n, NUM3);
    result += floor_mul_div(n, NUM4);
    result += floor_mul_div(n, NUM5);
    return result;
}
