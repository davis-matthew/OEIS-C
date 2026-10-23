/*
 * A121718  Write 0, 1, ..., n in base 3 and add as if they were decimal
 *          numbers.
 * https://oeis.org/A121718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ_{k=0..n} decimal_value_of_base3(k)
 * Range:     u8 n <= 9, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result  = 0;
    seq_t pow3    = 1;          /* 3^p */
    seq_t pow10   = 1;          /* 10^p */
    seq_t n1      = (seq_t)n + 1; /* amount of terms */

    /* Process digits while 3^p ≤ n (higher positions would be all zeros) */
    while (pow3 <= (seq_t)n)
    {
        seq_t cycleLen    = pow3 * 3;                /* 3^(p+1) */
        seq_t fullCycles  = n1 / cycleLen;           /* complete cycles */
        seq_t r           = n1 % cycleLen;           /* remaining numbers */

        /* Sum of digits at position p from full cycles */
        seq_t sumDigits   = fullCycles * (3 * pow3); /* (0+1+2)*3^p per cycle */

        /* Remainder contribution */
        seq_t remGroups   = r / pow3;                /* 0,1,or 2 full groups */
        seq_t partial     = r % pow3;                /* numbers in the partial group */
        seq_t remSum      = pow3 * (remGroups * (remGroups - 1) / 2)
                           + remGroups * partial;    /* sum of digits from remainder */

        sumDigits += remSum;
        result += sumDigits * pow10;

        /* Move to next digit position */
        pow3  *= 3;
        pow10 *= 10;
    }

    return result;
}
